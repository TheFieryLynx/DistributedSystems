#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROC_NUM 16
#define PRE_ELECTION 50
#define ELECTIONS 100
#define OK 200


int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != PROC_NUM) {
        printf("This application is meant to be run with %d processes.\n", PROC_NUM);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    
    // Если не был введен номер начального координатора
    if (argc != 3) {
        if (rank == 0) printf("Не введен номер начального координатора");
        MPI_Finalize();
        return 0;
    }

    int coordinator_num = atoi(argv[1]); //номер координатора от 0 до size - 1

    //printf("%d", coordinator_num);
    MPI_Barrier(MPI_COMM_WORLD);
    
    char file_name[100];
    sprintf(file_name, "%s/%d.txt", argv[2], rank);
    printf("%s\n", file_name);
    FILE *fp;
    if ((fp = fopen(file_name, "w+t")) == NULL) {
        printf("Cannot open file.\n");
    } else {
        printf("file opened.\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank != coordinator_num) {
        
        int message_send = rank;
        int message_get;
        
        MPI_Request request;
        MPI_Status status;
        
        int coordinator_is_alive = 1;
        while(coordinator_is_alive){
            printf("%d SEND message %d (start)\n", rank, message_send);
            fprintf(fp, "SENDING message %d (start)\n", message_send);
            MPI_Send(&message_send, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD);
            printf("%d SEND message (stop)\n", rank);
            fprintf(fp, "SENDING message (stop)\n\n");
            
            printf("--- %d RECV message (start)\n", rank);
            fprintf(fp, "RECIEVING message (start)\n");
            //MPI_Recv(&message_get, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD, &status);
            MPI_Irecv(&message_get, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD, &request);
            int flag = 0;
            double start = MPI_Wtime();
            
            while (!flag) {
                double current_time = MPI_Wtime();
                MPI_Test(&request, &flag, &status);
                if (current_time - start > 1) {
                    printf("proc %d detected that coordinator %d is not responding\n", rank, coordinator_num);
                    fprintf(fp, "Detected that coordinator %d is not responding\n", coordinator_num);
                    coordinator_is_alive = 0;
                    break;
                }
            }
            if (coordinator_is_alive) {
                printf("--- %d RECV message %d (stop)\n", rank, message_get);
                fprintf(fp, "RECIEVING message %d (stop)\n\n", message_get);
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
            //printf("%lf", current_time - start);
            if (current_time - start < 1){              //время жизни координатора
                printf("--------- %d RECV message (start)\n", rank);
                fprintf(fp, "RECIEVING message (start)\n");
                MPI_Recv(&message_get, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                printf("--------- %d RECV message %d (stop)\n", rank, message_get);
                fprintf(fp, "RECIEVING message %d (stop)\n\n", message_get);
                message_send = message_get;
                
                printf("----------------- %d SEND message %d (start)\n", rank, message_send);
                fprintf(fp, "SENDING message %d (start)\n", message_send);
                MPI_Send(&message_send, 1, MPI_INT, message_get, 0, MPI_COMM_WORLD);
                printf("----------------- %d SEND message (stop)\n", rank);
                fprintf(fp, "SENDING message (stop)\n\n");
                
            } else {
                //MPI_Free_mem(baseptr);
                printf("%d IS DYING\n", rank);
                fprintf(fp, "I'M DYING\n\n");
                fclose(fp);
                MPI_Finalize();
                exit(0);
            }
        }
    }
    
    int send_arr[2] = {-1, -1};
    int recv_arr[2] = {-1, -1};
    
    printf("%d STARTING ELECTIONS\n", rank);
    fprintf(fp, "STARTING ELECTIONS\n");
    int cur_status = PRE_ELECTION;
    
    MPI_Status status;
    while (1) {
        if (cur_status == PRE_ELECTION) {
            for (int i = rank + 1; i < PROC_NUM; ++i) {
                send_arr[0] = rank;
                send_arr[1] = ELECTIONS;
                printf("%d SEND message %d %d(start)\n", rank, send_arr[0], send_arr[1]);
                fprintf(fp, "SENDING message %d %d(start)\n", send_arr[0], send_arr[1]);
                MPI_Send(send_arr, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
                printf("%d SEND message (stop)\n", rank);
                fprintf(fp, "SENDING message (stop)\n\n");
            }
            cur_status = ELECTIONS;
        }
        if (cur_status == ELECTIONS) {
            
            // Irecv
//            MPI_Recv(recv_arr, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            // MPI_Irecv(recv_arr, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            
            MPI_Request request;
            printf("********* %d RECV message (start)\n", rank);
            fprintf(fp, "RECIEVING message (start)\n");
            //MPI_Recv(&message_get, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD, &status);
            MPI_Irecv(recv_arr, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            int flag = 0;
            double start = MPI_Wtime();
            
            int elections_continue = 1;
            
            while (!flag) {
                double current_time = MPI_Wtime();
                MPI_Test(&request, &flag, &status);
                if (current_time - start > 1) {
                    printf("%d didn't get a response OK from any proc\n", rank);
                    fprintf(fp, "didn't get a response OK from any proc\n");
                    elections_continue = 0;
                    break;
                }
            }
            if (elections_continue) {
                printf("********* %d RECV message %d %d(stop)\n", rank, recv_arr[0], recv_arr[1]);
                fprintf(fp, "RECIEVING message %d %d(stop)\n\n", recv_arr[0], recv_arr[1]);
            } else {
                break;
            }
            if (recv_arr[1] == ELECTIONS) {
                send_arr[0] = rank;
                send_arr[1] = OK;
                printf("%d SEND message %d %d(start)\n", rank, send_arr[0], send_arr[1]);
                fprintf(fp, "SENDING message %d %d(start)\n", send_arr[0], send_arr[1]);
                MPI_Send(send_arr, 2, MPI_INT, recv_arr[0], 0, MPI_COMM_WORLD);
                printf("%d SEND message (stop)\n", rank);
                fprintf(fp, "SENDING message (stop)\n\n");
            }
            if (recv_arr[1] == OK) {
                printf("%d ELECTIONS STOP because of %d\n", rank, recv_arr[0]);
                fprintf(fp, "ELECTIONS STOP because of %d\n\n", recv_arr[0]);
                break;
            }
        }
    }
    
    
    
    
//    MPI_Request request;
    
//    while (1) {
//        printf("********* %d RECV message (start)\n", rank);
//        fprintf(fp, "RECIEVING message (start)\n");
//        // Irecv
//        MPI_Recv(recv_arr, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
//        // MPI_Irecv(recv_arr, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
//        printf("********* %d RECV message %d %d(stop)\n", rank, recv_arr[0], recv_arr[1]);
//        fprintf(fp, "RECIEVING message %d %d(stop)\n\n", recv_arr[0], recv_arr[1]);
//
//    }

    
    
    
    
    
    
    
    
    // Print off a hello world message
    if (rank != coordinator_num) {
        //printf("Hello world from rank %d out of %d processors\n", rank, size);
        // Finalize the MPI environment.
        //MPI_Free_mem(baseptr);
        fclose(fp);
        MPI_Finalize();
        exit(0);
    }
}
