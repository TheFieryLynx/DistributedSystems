#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <ctime>
#include <chrono>
#include <string>
#include <fstream> 
#include <sstream>
using namespace std::chrono;

steady_clock::time_point start_time, end_time;

void timer_start( void )
{
    start_time = steady_clock::now();
}

void timer_stop( void )
{
    end_time = steady_clock::now();
}

void timer_print( void )
{
    duration<double> time =  end_time - start_time;
    std::cout << "Runtime: " << time.count()  << std::endl;
}

void swap(std::vector<int>::iterator ii, std::vector<int>::iterator jj) 
{
    int tmp = *ii;
    *ii = *jj;
    *jj = tmp;
}

void check_sorted(std::vector<int>::iterator ii, int size) 
{
    for(int i = 0; i < size - 1; i++) {
        if(*ii > *(ii + 1)) {
            std::cout << "Bad sorting";
            return;
        }
    }
    std::cout << "Sorted successfully! :)" << std::endl;
}

void print(std::vector<int>::iterator ii, int size)
{
    for(int i = 0; i < size; i++) {
        std::cout << *(ii + i) << " ";
    }
    std::cout << std::endl << std::endl;
}

void merge(std::vector<int>::iterator ii, std::vector<int>::iterator jj, int size)
{
    int size_a = size / 2, size_b = size - size / 2;
    int i_a = 0, i_b = 0;
    std::vector<int>::iterator ii2 = ii + size_a;
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
    for(int i = 0; i < size; i++) {
        *(ii + i) = *(jj + i);
    }
}

void small_sort(std::vector<int>::iterator ii, int size)
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


void merge_serial(std::vector<int>::iterator ii, std::vector<int>::iterator jj, int size)
{
    if (size <= 3) {
        small_sort(ii, size);
        return;
    }
    merge_serial(ii, jj, size / 2);
    merge_serial(ii + size / 2, jj + size / 2, size - size / 2);
    merge(ii, jj, size);
}

void merge_parallel(std::vector<int>::iterator ii, std::vector<int>::iterator jj, int threads, int size)
{
    if (threads == 1) {
        merge_serial(ii, jj, size);
    } else if (threads > 1) {
        #pragma omp task
        merge_parallel(ii, jj, threads / 2, size / 2);
        merge_parallel(ii + size / 2, jj + size / 2, threads - threads / 2, size - size / 2);
        #pragma omp taskwait
        merge(ii, jj, size);
    }
}

int main(int argc, char *argv[])
{
    srand(1425);
    int n = 0;
    sscanf(argv[1], "%d", &n);
    std::vector<int> v(n);
    for(int i = 0; i < n; ++i) {
        v[i] = rand() % 100;
    }
    int num_of_threats;
    sscanf(argv[2], "%d", &num_of_threats);
    std::vector<int> result(n);

    auto ii = v.begin();
    auto jj = result.begin();
    timer_start();
    #pragma omp parallel
    {
        #pragma omp single
        {
            merge_parallel(ii, jj, num_of_threats, v.size());
        }
    }

    timer_stop();
    timer_print();
    check_sorted(result.begin(), n);
}

