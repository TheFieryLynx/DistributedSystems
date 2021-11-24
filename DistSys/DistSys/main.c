#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROC_NUM 16
#define PRE_ELECTION 50
#define ELECTIONS 100
#define OK 200

#define PING_PONG_TAG 500
#define ELECTION_TAG 600
#define MAILING_TAG 700

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != PROC_NUM) {
        printf("This application is meant to be run with %d processes.\n", PROC_NUM);
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    
    // Если не был введен номер начального координатора
    if (argc != 3) {
        if (rank == 0) printf("Не введен номер начального координатора");
        MPI_Finalize();
        return 0;
    }

    int coordinator_num = atoi(argv[1]); //номер координатора от 0 до size - 1

    MPI_Barrier(MPI_COMM_WORLD);
    
    char file_name[100];
    sprintf(file_name, "%s/%d.txt", argv[2], rank);
    printf("%s\n", file_name);
    fflush(stdout);
    FILE *fp;
    if ((fp = fopen(file_name, "w+t")) == NULL) {
        printf("Cannot open file.\n");
        fflush(stdout);
    } else {
        printf("file opened.\n");
        fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    // -------------------------<PING PONG (START)>-------------------------
    
    if (rank != coordinator_num) {
        
        int message_send = rank;
        int message_get;
        
        MPI_Request request;
        MPI_Status status;
        
        int coordinator_is_alive = 1;
        while(coordinator_is_alive){
            printf("[%d]: SENDING message %d (start)\n", rank, message_send);
            fprintf(fp, "SENDING message %d (start)\n", message_send);
            fflush(stdout);
            MPI_Send(&message_send, 1, MPI_INT, coordinator_num, PING_PONG_TAG, MPI_COMM_WORLD);
            printf("[%d]: SENDING message (stop)\n", rank);
            fprintf(fp, "SENDING message (stop)\n\n");
            fflush(stdout);
            
            printf("--- [%d]: RECIEVING message (start)\n", rank);
            fprintf(fp, "RECIEVING message (start)\n");
            fflush(stdout);
            MPI_Irecv(&message_get, 1, MPI_INT, coordinator_num, PING_PONG_TAG, MPI_COMM_WORLD, &request);
            int flag = 0;
            double start = MPI_Wtime();
            
            while (!flag) {
                double current_time = MPI_Wtime();
                MPI_Test(&request, &flag, &status);
                if (current_time - start > 1) {
                    printf("proc %d detected that coordinator %d is not responding\n", rank, coordinator_num);
                    fprintf(fp, "Detected that coordinator %d is not responding\n", coordinator_num);
                    fflush(stdout);
                    coordinator_is_alive = 0;
                    break;
                }
            }
            if (coordinator_is_alive) {
                printf("--- [%d]: RECIEVING message %d (stop)\n", rank, message_get);
                fprintf(fp, "RECIEVING message %d (stop)\n\n", message_get);
                fflush(stdout);
            }
        }
    }
    if (rank == coordinator_num){
        double start = MPI_Wtime();
        MPI_Status status;
        int message_get;
        int message_send;
        while (1) {
            double current_time = MPI_Wtime();
            if (current_time - start < 1){              //время жизни координатора
                printf("--------- [%d]: RECIEVING message (start)\n", rank);
                fprintf(fp, "RECIEVING message (start)\n");
                fflush(stdout);
                MPI_Recv(&message_get, 1, MPI_INT, MPI_ANY_SOURCE, PING_PONG_TAG, MPI_COMM_WORLD, &status);
                printf("--------- [%d]: RECIEVING message %d (stop)\n", rank, message_get);
                fprintf(fp, "RECIEVING message %d (stop)\n\n", message_get);
                fflush(stdout);
                message_send = message_get;
                
                printf("----------------- [%d]: SENDING message %d (start)\n", rank, message_send);
                fprintf(fp, "SENDING message %d (start)\n", message_send);
                fflush(stdout);
                MPI_Send(&message_send, 1, MPI_INT, message_get, PING_PONG_TAG, MPI_COMM_WORLD);
                printf("----------------- [%d]: SENDING message (stop)\n", rank);
                fprintf(fp, "SENDING message (stop)\n\n");
                fflush(stdout);
                
            } else {
                printf("%d IS DYING\n", rank);
                fprintf(fp, "I'M DYING\n\n");
                fflush(stdout);
                fclose(fp);
                MPI_Finalize();
                exit(0);
            }
        }
    }
    // -------------------------<PING PONG (STOP)>-------------------------
    // -------------------------<ELECTIONS (START)>-------------------------
    int send_arr[2] = {-1, -1};
    int recv_arr[2] = {-1, -1};
    
    printf("%d STARTING ELECTIONS\n", rank);
    fprintf(fp, "STARTING ELECTIONS\n");
    int cur_status = PRE_ELECTION;
    
    int new_coordinator_num = coordinator_num;
    
    MPI_Status status;
    while (1) {
        if (cur_status == PRE_ELECTION) {
            for (int i = rank + 1; i < PROC_NUM; ++i) {
                send_arr[0] = rank;
                send_arr[1] = ELECTIONS;
                printf("[%d]: SENDING message %d %d(start)\n", rank, send_arr[0], send_arr[1]);
                fprintf(fp, "SENDING message %d %d(start)\n", send_arr[0], send_arr[1]);
                fflush(stdout);
                MPI_Send(send_arr, 2, MPI_INT, i, ELECTION_TAG, MPI_COMM_WORLD);
                printf("[%d]: SENDING message (stop)\n", rank);
                fprintf(fp, "SENDING message (stop)\n\n");
                fflush(stdout);
            }
            cur_status = ELECTIONS;
        }
        if (cur_status == ELECTIONS) {
            MPI_Request request;
            printf("********* [%d]: RECIEVING message (start)\n", rank);
            fprintf(fp, "RECIEVING message (start)\n");
            fflush(stdout);
            MPI_Irecv(recv_arr, 2, MPI_INT, MPI_ANY_SOURCE, ELECTION_TAG, MPI_COMM_WORLD, &request);
            int flag = 0;
            double start = MPI_Wtime();
            
            int elections_continue = 1;
            
            while (!flag) {
                double current_time = MPI_Wtime();
                MPI_Test(&request, &flag, &status);
                if (current_time - start > 1) {
                    printf("%d didn't get a response OK from any proc\n", rank);
                    fprintf(fp, "didn't get a response OK from any proc\n");
                    fflush(stdout);
                    elections_continue = 0;
                    new_coordinator_num = rank;

                    break;
                }
            }
            if (elections_continue) {
                printf("********* %d RECV message %d %d(stop)\n", rank, recv_arr[0], recv_arr[1]);
                fprintf(fp, "RECIEVING message %d %d(stop)\n\n", recv_arr[0], recv_arr[1]);
                fflush(stdout);
            } else {
                break;
            }
            if (recv_arr[1] == ELECTIONS) {
                send_arr[0] = rank;
                send_arr[1] = OK;
                printf("[%d]: SENDING message %d %d(start)\n", rank, send_arr[0], send_arr[1]);
                fprintf(fp, "SENDING message %d %d(start)\n", send_arr[0], send_arr[1]);
                fflush(stdout);
                MPI_Send(send_arr, 2, MPI_INT, recv_arr[0], ELECTION_TAG, MPI_COMM_WORLD);
                printf("[%d]: SENDING message (stop)\n", rank);
                fprintf(fp, "SENDING message (stop)\n\n");
                fflush(stdout);
            }
            if (recv_arr[1] == OK) {
                printf("[%d]: ELECTIONS STOP because of %d\n", rank, recv_arr[0]);
                fprintf(fp, "ELECTIONS STOP because of %d\n\n", recv_arr[0]);
                fflush(stdout);
                break;
            }
        }
    }
    
    // -------------------------<ELECTIONS (START)>-------------------------
    // -------------------------<MAILING NEW COORDINATOR RANK (START)>-------------------------
    
    printf("[%d]: previous Coordinator is %d\n", rank, coordinator_num);
    fprintf(fp, "previous Coordinator is %d\n\n", coordinator_num);
    fflush(stdout);
    
    int message_get = 0;
    if (rank == new_coordinator_num) {
        for (int i = 0; i < PROC_NUM; ++i) {
            if (i != coordinator_num && i != new_coordinator_num) {
                printf("[%d]: SENDING new_coordinator_num %d to %d(start)\n", rank, new_coordinator_num, i);
                fprintf(fp, "SENDING new_coordinator_num %d to %d(start)\n", new_coordinator_num, i);
                fflush(stdout);
                MPI_Send(&new_coordinator_num, 1, MPI_INT, i, MAILING_TAG, MPI_COMM_WORLD);
                printf("[%d]: SENDING new_coordinator_num (stop)\n", rank);
                fprintf(fp, "SENDING new_coordinator_num %d to %d(start)\n", new_coordinator_num, i);
                fflush(stdout);
                coordinator_num = new_coordinator_num;
            }
        }
    } else {
        printf("[%d]: RECIEVING new_coordinator_num (start)\n", rank);
        fprintf(fp, "RECIEVING new_coordinator_num (start)\n");
        MPI_Recv(&message_get, 1, MPI_INT, MPI_ANY_SOURCE, MAILING_TAG, MPI_COMM_WORLD, &status);
        printf("[%d]: RECIEVING new_coordinator_num %d (stop)\n", rank, message_get);
        fprintf(fp, "RECIEVING new_coordinator_num %d (stop)\n\n", message_get);
        fflush(stdout);
        coordinator_num = message_get;
    }
    
    // -------------------------<MAILING NEW COORDINATOR RANK (STOP)>-------------------------
    
    printf("[%d]: NEW COORDINATOR IS %d (stop)\n", rank, coordinator_num);
    fprintf(fp, "RECIEVING new_coordinator_num %d (stop)\n\n", coordinator_num);
    fflush(stdout);
    fclose(fp);
    MPI_Finalize();
    exit(0);
}
