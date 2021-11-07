#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    if (argc != 2) {
        if (rank == 0) printf("Не введен номер начального координатора");
        MPI_Finalize();
        return 0;
    }

    int coordinator_num = atoi(argv[1]); //номер координатора от 0 до size - 1

    //printf("%d", coordinator_num);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status status;
    
    
    //printf("Hello world from rank %d out of %d processors\n", rank, size);
    if (rank != coordinator_num) {
        int message_send = rank;
        int message_get;
        while(1){
            printf("%d SEND message %d (start)\n", rank, message_send);
            MPI_Send(&message_send, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD);
            printf("%d SEND message (stop)\n", rank);
            
            
        
            printf("--- %d RECV message (start)\n", rank);
            MPI_Recv(&message_get, 1, MPI_INT, coordinator_num, 0, MPI_COMM_WORLD, &status);
            printf("--- %d RECV message %d (stop)\n", rank, message_get);
            
        }
        
    }
    if (rank == coordinator_num){
        double start = MPI_Wtime();
        int message_get;
        int message_send;
        int i = 30;
        while (1) {
            double current_time = MPI_Wtime();
            //printf("%lf", current_time - start);
            if (current_time - start < 5){
                printf("--------- %d RECV message (start)\n", rank);
                MPI_Recv(&message_get, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                printf("--------- %d RECV message %d (stop)\n", rank, message_get);
                
                message_send = message_get;
                
                printf("----------------- %d SEND message %d (start)\n", rank, message_send);
                MPI_Send(&message_send, 1, MPI_INT, message_get, 0, MPI_COMM_WORLD);
                printf("----------------- %d SEND message (stop)\n", rank);
                
            } else {
                MPI_Finalize();
            }
        }
                
        
    }

   



    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    //printf("Hello world from rank %d out of %d processors\n", rank, size);
    // Finalize the MPI environment.
    MPI_Finalize();
}
