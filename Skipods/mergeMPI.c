#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

double t_start, t_end;

void timer_start( void )
{
    t_start = MPI_Wtime();
}

void timer_stop( void )
{
    t_end = MPI_Wtime();
}

void timer_print( void )
{
    printf( "Time in seconds = %0.6lf\n", t_end - t_start );
}

void swap(int *ii, int *jj) 
{
    int tmp = *ii;
    *ii = *jj;
    *jj = tmp;
}

void print(int *a, int size)
{
    int i;
    for(i = 0; i < size; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void check_sorted(int *ii, int size) 
{
    int i;
    for(i = 0; i < size - 1; i++) {
        if(*ii > *(ii + i)) {
            printf("Bad sorting! :( \n");;
            return;
        }
    }
   printf("Sorted successfully! :)\n");
}

void merge(int *ii, int *jj, int size)
{
    int size_a = size / 2, size_b = size - size / 2;
    int i_a = 0, i_b = 0;
    int *ii2 = ii + size_a;
    while (i_a < size_a && i_b < size_b) {
        if (*(ii + i_a) < *(ii2 + i_b)) {
            *(jj + i_a + i_b) = *(ii + i_a);
            ++i_a;
        } else {
            *(jj + i_a + i_b) = *(ii2 + i_b);
            ++i_b;
        }
    }
    while (i_a < size_a) {
        *(jj + i_a + i_b) = *(ii + i_a);
        ++i_a;
    }
    while (i_b < size_b) {
        *(jj + i_a + i_b) = *(ii2 + i_b);
        ++i_b;
    }
    int i;
    for(i = 0; i < size; i++) {
        *(ii + i) = *(jj + i);
    }
}

void small_sort(int *ii, int size)
{
    if (size <= 1) {
        return;
    } else if (size == 2) {
        if (*ii > *(ii + 1)) {
            swap(ii, ii + 1);
        }
    }
    else if (size == 3) {
        if (*ii > *(ii + 1)) {
            swap(ii, ii + 1);
        }
        if (*ii > *(ii + 2)) {
            swap(ii, ii + 2);
        }
        if (*(ii + 1) > *(ii + 2)) {
            swap(ii + 1, ii + 2);
        }
    }
}

void merge_serial(int *ii, int *jj, int size)
{
    if (size <= 3) {
        small_sort(ii, size);
        return;
    }
    merge_serial(ii, jj, size / 2);
    merge_serial(ii + size / 2, jj + size / 2, size - size / 2);
    merge(ii, jj, size);
}

void mpi_mergesort_parallel(int a[], int size, int tmp[], int level, int my_rank, int max_rank, int tag, MPI_Comm comm)
{
    int helper_rank = my_rank + pow (2, level);
    if(helper_rank > max_rank) {				
        merge_serial (a, tmp, size);
    } else {	
        MPI_Request request;
        MPI_Status status;
        MPI_Isend (a + size / 2, size - size / 2, MPI_INT, helper_rank, tag, comm, &request);
        mpi_mergesort_parallel(a, size / 2, tmp, level + 1, my_rank, max_rank, tag, comm);
        MPI_Request_free (&request);
        MPI_Recv (a + size / 2, size - size / 2, MPI_INT, helper_rank, tag, comm, &status);
        merge (a, tmp, size);
    }
    return;
}

void mpi_root(int a[], int size, int tmp[], int max_rank, int tag, MPI_Comm comm)
{
    int my_rank;
    MPI_Comm_rank (comm, &my_rank);
    if (my_rank != 0) {
        printf("Error");
        MPI_Abort (MPI_COMM_WORLD, 1);
    }
    mpi_mergesort_parallel(a, size, tmp, 0, my_rank, max_rank, tag, comm);
    return;
}

void mpi_helper(int my_rank, int max_rank, int tag, MPI_Comm comm)
{
    int level = 0, size;
    while(pow (2, level) <= my_rank) {
        level++;
    }
    MPI_Status status;
    MPI_Probe (MPI_ANY_SOURCE, tag, comm, &status);
    MPI_Get_count (&status, MPI_INT, &size);
    int parent_rank = status.MPI_SOURCE;
    int *a = calloc (size, sizeof (int));
    int *tmp = calloc (size, sizeof (int));

    MPI_Recv (a, size, MPI_INT, parent_rank, tag, comm, &status);
    mpi_mergesort_parallel(a, size, tmp, level, my_rank, max_rank, tag, comm);
    MPI_Send (a, size, MPI_INT, parent_rank, tag, comm);
    return;
}

int main(int argc, char **argv) {
    int mpi_comm_rank, mpi_comm_size;
    int *orig, *tmp;
    MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_comm_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_comm_size);
    int n = atoi(argv[1]);
    if (!mpi_comm_rank) {
        orig = calloc(n, sizeof(int));
        tmp = calloc(n, sizeof(int));
        int i;
        for(i = 0; i < n; ++i) {
            orig[i] = rand() % 1000;
            //orig[i] = n - i;
        }
        //print(orig, n);
    }
    timer_start();
    int max_mpi_comm_rank = mpi_comm_size - 1;
    int tag = 0xABCDEF;
    if(!mpi_comm_rank) {
        mpi_root(orig, n, tmp, max_mpi_comm_rank, tag, MPI_COMM_WORLD);
    } else {
        mpi_helper(mpi_comm_rank, max_mpi_comm_rank, tag, MPI_COMM_WORLD);
    }

    if (!mpi_comm_rank) {
        printf("%d\n", mpi_comm_size);
        timer_stop();
        timer_print();
        check_sorted(orig, n);
        //print(orig, n);
        free(orig);
        free(tmp);
    }

    fflush (stdout);
    MPI_Finalize ();
    return 0;  
}