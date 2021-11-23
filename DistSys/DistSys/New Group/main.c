#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 16) {
        printf("This application is meant to be run with %d processes.\n", 16);
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
            MPI_Send(&message_send, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD);
            printf("%d SEND message (stop)\n", rank);
            
            printf("--- %d RECV message (start)\n", rank);
            //MPI_Recv(&message_get, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD, &status);
            MPI_Irecv(&message_get, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD, &request);
            int flag = 0;
            double start = MPI_Wtime();
            
            while (!flag) {
                double current_time = MPI_Wtime();
                MPI_Test(&request, &flag, &status);
                if (current_time - start > 5) {
                    printf("proc %d detected that coordinator %d is not responding\n", rank, coordinator_num);
                    coordinator_is_alive = 0;
                    break;
                }
            }
            if (coordinator_is_alive) printf("--- %d RECV message %d (stop)\n", rank, message_get);
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
            if (current_time - start < 10){
                printf("--------- %d RECV message (start)\n", rank);
                MPI_Recv(&message_get, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                printf("--------- %d RECV message %d (stop)\n", rank, message_get);
                
                message_send = message_get;
                
                printf("----------------- %d SEND message %d (start)\n", rank, message_send);
                MPI_Send(&message_send, 1, MPI_INT, message_get, 0, MPI_COMM_WORLD);
                printf("----------------- %d SEND message (stop)\n", rank);
                
            } else {
                //MPI_Free_mem(baseptr);
                MPI_Finalize();
                exit(0);
            }
        }
    }
    
   
   
   



    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    if (rank != coordinator_num) {
        //printf("Hello world from rank %d out of %d processors\n", rank, size);
    // Finalize the MPI environment.
        //MPI_Free_mem(baseptr);
        MPI_Finalize();
        
        exit(0);
    }
}
