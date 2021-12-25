#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>
#include <mpi-ext.h>
#include <signal.h>

#define KILL_PROC 4
#define COORDINATOR_NUM 0

MPI_Comm mpi_comm_world_custom;
bool error_occured = false;
char filename[20];
int rank, size;
int splitted_size;
char killed_filename[20];
char new_killed_filename[20] = "./logs/reserve.txt";

void print(int rank, int *a, int size)
{   
    printf("rank %d:", rank);
    int i;
    for(i = 0; i < size; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void swap(int *ii, int *jj) 
{
    int tmp = *ii;
    *ii = *jj;
    *jj = tmp;
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

void mergeFinal(int *ii, int *jj, int *tmp, int size_ii, int size_jj) {
    int i_a = 0, i_b = 0;
    while (i_a < size_ii && i_b < size_jj) {
        if (*(ii + i_a) < *(jj + i_b)) {
            *(tmp + i_a + i_b) = *(ii + i_a);
            ++i_a;
        } else {
            *(tmp + i_a + i_b) = *(jj + i_b);
            ++i_b;
        }
    }
    while (i_a < size_ii) {
        *(tmp + i_a + i_b) = *(ii + i_a);
        ++i_a;
    }
    while (i_b < size_jj) {
        *(tmp + i_a + i_b) = *(jj + i_b);
        ++i_b;
    }
    int i;
    for(i = 0; i < size_ii + size_jj; i++) {
        *(ii + i) = *(tmp + i);
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

void bubbleSort(int arr[], int n)
{
   int i, j;
   for (i = 0; i < n-1; i++)     
 
       for (j = 0; j < n-i-1; j++)
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
}

void check_sort (int *check, int *arr, int size)
{
    bubbleSort(check, size);
    for(int i = 0; i < size; ++i) {
        if (check[i] != arr[i]) {
            printf("Sorting is not correct");
            return;
        }
    }
    printf("Sorted successfully! :)\n");
}

void save_into_file(char* filename, int *arr, int size)
{
    //print(0, arr,size);
    FILE *fd = fopen(filename, "w");
    for(int i = 0; i < size; ++i) {
        fprintf(fd, "%d ", arr[i]);
    }
    fclose(fd);
}

void read_from_file(char* filename, int *arr, int size)
{
    FILE *fd = fopen(filename, "r");
    for(int i = 0; i < size; ++i) {
        fscanf(fd, "%d", &arr[i]);
    }
    fclose(fd);
}

static void verbose_errhandler (MPI_Comm *comm, int *err, ...) {
    char errstr[MPI_MAX_ERROR_STRING];
    int size, num_failed, len;
    MPI_Group group_failed;
    int old_rank = rank;
    error_occured = true;
    MPI_Comm_size(mpi_comm_world_custom, &size);
    MPIX_Comm_failure_ack(mpi_comm_world_custom);
    MPIX_Comm_failure_get_acked(mpi_comm_world_custom, &group_failed);
    MPI_Group_size(group_failed, &num_failed);
    MPI_Error_string(*err, errstr, &len);

    MPIX_Comm_shrink(mpi_comm_world_custom, &mpi_comm_world_custom);
    MPI_Comm_rank(mpi_comm_world_custom, &rank);
    MPI_Comm_size(mpi_comm_world_custom, &size);
    printf("New rank for process %d: %d\n", old_rank, rank);
    MPI_Barrier(mpi_comm_world_custom);
    int * ranks = malloc(sizeof(int)*size);
    MPI_Gather(&old_rank, 1, MPI_INT, ranks, 1, MPI_INT, 0, mpi_comm_world_custom);
    if (rank == COORDINATOR_NUM) {
        int killed_proc_num;
        for (int i = 0; i < size - 1; ++i) {
            if (ranks[i + 1] - ranks[i] > 1) {
                killed_proc_num = ranks[i] + 1;
            }
        }
        printf("Killed proc: %d\n", killed_proc_num);
        sprintf(killed_filename, "./logs/%d.txt", killed_proc_num);
        int *read_array = calloc(splitted_size, sizeof(int));
        read_from_file(killed_filename, read_array, splitted_size);
        int *tmp_splitted_array = calloc(splitted_size, sizeof(int));
        merge_serial(read_array, tmp_splitted_array, splitted_size);
        save_into_file(new_killed_filename, read_array, splitted_size);
    }
    
    

}   

int main(int argc, char **argv) {

    int n = atoi(argv[1]);
    int *unsorted = calloc(n, sizeof(int));
    int *check = calloc(n, sizeof(int));
    int i;
    for(i = 0; i < n; ++i) {
        int a = rand() % 1000;
        unsorted[i] = a;
        check[i] = a;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    mpi_comm_world_custom = MPI_COMM_WORLD;

    MPI_Errhandler errh;
    MPI_Comm_create_errhandler(verbose_errhandler, &errh);
    MPI_Comm_set_errhandler(MPI_COMM_WORLD, errh);
    MPI_Barrier(mpi_comm_world_custom);

 
    sprintf(filename, "./logs/%d.txt", rank);

    splitted_size = n / size;
    int delta = n - splitted_size * size;
    
    int *splitted_array = calloc(splitted_size, sizeof(int));
    int *tmp_splitted_array = calloc(splitted_size, sizeof(int));

    MPI_Scatter(unsorted, splitted_size, MPI_INT, splitted_array, splitted_size, MPI_INT, 0, mpi_comm_world_custom);
    save_into_file(filename, splitted_array, splitted_size);
    MPI_Barrier(mpi_comm_world_custom);

    merge_serial(splitted_array, tmp_splitted_array, splitted_size);
    if (rank == KILL_PROC) {
        raise(SIGKILL);
    }
    MPI_Barrier(mpi_comm_world_custom);
    save_into_file(filename, splitted_array, splitted_size);
    int *read_array = calloc(splitted_size, sizeof(int));
    read_from_file(filename, read_array, splitted_size);

    int *pre_sorted = NULL;
    if (rank == COORDINATOR_NUM) {
        pre_sorted = calloc(n, sizeof(int));
    }
    MPI_Gather(read_array, splitted_size, MPI_INT, pre_sorted, splitted_size, MPI_INT, 0, mpi_comm_world_custom);
  
    
    if (rank == COORDINATOR_NUM) {
        if (error_occured) {
            read_from_file(new_killed_filename, pre_sorted + n - splitted_size - delta, splitted_size);
        }
        int *sorted_tale = NULL;
        printf("DELTA %d\n", delta);
        if(delta > 0) {
            sorted_tale = calloc(delta, sizeof(int));
            merge_serial(unsorted + n - delta, sorted_tale, delta);
            for(int i = n - delta; i < n; ++i) {
                pre_sorted[i] = unsorted[i];
            }
            free(sorted_tale);
        }
        
        int *tmp = calloc(n, sizeof(int));

        int i;
        for(i = 1; i < size; ++i) {
            int s = splitted_size * i;
            mergeFinal(pre_sorted, pre_sorted + s, tmp, s, splitted_size);
        }
        if (delta > 0) {
            int s1 = i * splitted_size;
            mergeFinal(pre_sorted, pre_sorted + s1, tmp, s1, delta);
        }
        check_sort(check, pre_sorted, n);
        free(tmp);
    }
    if (pre_sorted != NULL) {
        free(pre_sorted);
    }
    free(unsorted);
    free(check);
    free(splitted_array);
    free(tmp_splitted_array);
    MPI_Finalize();
}