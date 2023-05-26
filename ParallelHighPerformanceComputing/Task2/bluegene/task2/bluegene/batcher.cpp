#include <iostream>
#include <vector>
#include <float.h>
#include <cmath>
#include "mpi.h"

int mpi_comm_size, mpi_comm_rank;
MPI_Datatype point_datatype;

// ---------------------<Sys>---------------------

struct Point {
    float coord[2];
    int index;
};

bool cmp(const Point &a, const Point &b) {
    return a.coord[0] < b.coord[0];
}

void print(std::vector<Point>& grid_proc) {
    MPI_Barrier(MPI_COMM_WORLD);
    for(int r = 0; r < mpi_comm_size; ++r) {
        if (mpi_comm_rank == r) {
            std::cout << "Proc " << mpi_comm_rank << std::endl;
            for(int i = 0; i < grid_proc.size(); ++i) {
                std::cout << grid_proc[i].coord[0] << " ";
            }
            std::cout << std::endl;
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    std::cout << std::endl;
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
}

void print_proc(std::vector<Point>& grid_proc) {
    for(int i = 0; i < grid_proc.size(); ++i) {
        std::cout << grid_proc[i].coord[0] << " ";
    }
    std::cout << std::endl;
    fflush(stdout);
}

void print_initial_info(int& n1, int& n2) 
{
    if (mpi_comm_rank == 0) {
        std::cout << "Number of processes: " << mpi_comm_size << std::endl;
        std::cout << "Params: " << n1 << ' ' << n2 << std::endl;
    }
    // std::cout << "Proc rank: " << mpi_comm_rank << std::endl;
    MPI_Barrier(MPI_COMM_WORLD);
}

void comparator(int a, int b) {
    std::cout << a << ' ' << b << std::endl;
}

// ---------------------<Create custom MPI datatype>---------------------
// https://rookiehpc.github.io/mpi/docs/mpi_type_create_struct/index.html

void create_point_datatype()
{
    int lengths[2] = {2, 1};

    MPI_Aint displacements[2];
    Point point;
    MPI_Aint base_address;

    MPI_Get_address(&point, &base_address);
    MPI_Get_address(&point.coord[0], &displacements[0]);
    MPI_Get_address(&point.index, &displacements[1]);

    // displacements[0] = MPI_Aint_diff(displacements[0], base_address);
    // displacements[1] = MPI_Aint_diff(displacements[1], base_address);

    displacements[0] = displacements[0] - base_address;
    displacements[1] = displacements[1] - base_address;

    MPI_Datatype types[2] = { MPI_FLOAT, MPI_INT };

    MPI_Type_create_struct(2, lengths, displacements, types, &point_datatype);
    MPI_Type_commit(&point_datatype);
}

// ---------------------<GenerateData>---------------------

float x(int& i, int& j) 
{
    return sin(i + 1) * cos(j + 1) * 10 * float(rand()) * float(10.0) / RAND_MAX;
}

float y(int& i, int& j) 
{
    return cos(j + 1) * sin(i + 1) * 10 * float(rand()) * float(10.0) / RAND_MAX;
}

Point create_grid_point(int& i, int& j, int& n1, int& n2)
{
    Point new_point;
    int index = i * n2 + j;
    if (index >= n1 * n2) {
        new_point.coord[0] = FLT_MAX;
        new_point.coord[1] = FLT_MAX;
        new_point.index = -1;
        return new_point;
    }
    new_point.coord[0] = x(i, j);
    new_point.coord[1] = y(i, j);
    new_point.index = index;
    return new_point;
}

void gen_grid_data(std::vector<Point>& grid, int n1, int n2)
{
    // P[i*n2+j].index = i*n2+j
    // P[i*n2+j].coord[0] = x(i, j), 
    // P[i*n2+j].coord[1] = y(i,j),  i = 0, …, n1-1, j = 0, …, n2-1

    int grid_size = n1 * n2;
    int proc_size = int(grid_size / mpi_comm_size);
    if (grid_size % mpi_comm_size != 0) {
        proc_size += 1;
    }
    for (int l = 0; l < proc_size; ++l) {
        int i = (mpi_comm_rank * proc_size + l) / n2;
        int j = (mpi_comm_rank * proc_size + l) % n2;
        grid.push_back(create_grid_point(i, j, n1, n2));
    }
}

// ---------------------<Merge sorted fragments>---------------------
// lecture 5 (74 - 75 slides)
// https://lira.imamod.ru/msu202209/L05_20221017_YakobovskiyMV.pdf 

void Join(const std::vector<Point>& a, const std::vector<Point>& b, std::vector<Point>& c, int rank1, int rank2) {
    int n = a.size();
    if (mpi_comm_rank == rank1) {
        for(int ia = 0, ib = 0, k = 0; k < n;) {
            if(a[ia].coord[0] < b[ib].coord[0]) {
                c[k++] = a[ia++];
            } else {
                c[k++] = b[ib++];
            }
        }
    } else if (mpi_comm_rank == rank2) {
        for(int ia = n - 1, ib = n - 1, k = n - 1; k >= 0;) {
            if(a[ia].coord[0] > b[ib].coord[0]) {
                c[k--] = a[ia--];
            } else {
                c[k--] = b[ib--];
            }
        }
    }
}

void processes_interaction(int rank1, int rank2, std::vector<Point>& points)
{
    if (mpi_comm_rank != rank1 && mpi_comm_rank != rank2) {
        return;
    }
    // rank1 - current proc, rank2 - other proc
    int other_rank, tag;
    std::vector<Point> points_from_proc(points.size());
    if (mpi_comm_rank == rank1) {
        other_rank = rank2;
        tag = rank1;
    } else {
        tag = rank2;
        other_rank = rank1;
    }
    // std::cout << rank1 + 1 << ' ' << rank2 + 1 << std::endl;
    // std::cout << "rank: " << mpi_comm_rank << " send " << other_rank << std::endl;
    MPI_Request request;
    MPI_Status status;
    MPI_Isend(points.data(), points.size(), point_datatype, other_rank, mpi_comm_rank, MPI_COMM_WORLD, &request);
    // MPI_Wait(&request, &status);
    MPI_Recv(points_from_proc.data(), points.size(), point_datatype, other_rank, other_rank, MPI_COMM_WORLD, &status);
    MPI_Wait(&request, &status);

    std::vector<Point> new_vec(points.size());
    Join(points, points_from_proc, new_vec, rank1, rank2);
    points = new_vec;
}

// ---------------------<Sort Network from first task>---------------------
void S(int first, int second, int step, int p1, int p2, std::vector<Point>& points) 
{
    if (p1 * p2 < 1) {
        return;
    }
    if (p1 == 1 && p2 == 1) {
        // comparator(first, second);
        processes_interaction(first, second, points);
        return; 
    }
    int p1_1 = p1 - p1 / 2; 
    int p2_1 = p2 - p2 / 2;
    S(first, second, 2 * step, p1_1 , p2_1, points);
    S(first + step, second + step, 2 * step, p1 - p1_1, p2 - p2_1, points);

    for(int i = 1; i < p1 - 1; i += 2) {
        // comparator(first + step * i, first + step * (i + 1));
        processes_interaction(first + step * i, first + step * (i + 1), points);
    }
    int ind = 0;
    if (p1 % 2 == 0) {
        //comparator(first + step *(p1 - 1), second);
        processes_interaction(first + step *(p1 - 1), second, points);
        ind = 1;
    }
    
    for(int i = ind; i < p2 - 1; i += 2) {
        //comparator(second + step * i, second + step *(i + 1));
        processes_interaction(second + step * i, second + step * (i + 1), points);
    }
}

void B(int first, int step, int count, std::vector<Point>& points) 
{
    if (count < 2) {
        return;
    }

    int count1 = count / 2;
    if (count % 2 != 0) {
        count1 += 1;
    }
    B(first, step, count1, points);
    B(first + step * count1, step, count - count1, points);
    S(first, first + step * count1, step, count1, count - count1, points);
}

// ---------------------<Main>---------------------

bool check(const std::vector<Point>& res) 
{
    for (int i = 1; i < res.size(); ++i) {
        if (res[i - 1].coord[0] > res[i].coord[0]) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_comm_rank);

    if (argc != 3 && mpi_comm_rank == 0) {
        std::cout << "---> ./prog n1 n2";
        exit(0);
    }
    srand(time(NULL) + mpi_comm_rank * 3210123);
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);

    std::vector<Point> grid_proc;
    print_initial_info(n1, n2);
    gen_grid_data(grid_proc, n1, n2);
    create_point_datatype();
    int first = 0;
    int step = 1;

    double start = MPI_Wtime();
    // sort on each proc 
    std::sort(grid_proc.begin(), grid_proc.end(), cmp);

    // run parallel sorting
   
    B(first, step, mpi_comm_size, grid_proc);
    double delta = MPI_Wtime() - start;
    double time;
    MPI_Reduce(&delta, &time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (mpi_comm_rank == 0) {
        std::cout << "Time: " << time << std::endl;
    }
    // let's collect all parts on proc with rank 0
    MPI_Barrier(MPI_COMM_WORLD);
    std::vector<Point> res;
    if (mpi_comm_rank == 0) {
        res = std::vector<Point>(mpi_comm_size * grid_proc.size());
    }
    MPI_Gather(grid_proc.data(), grid_proc.size(), point_datatype, res.data(), grid_proc.size(), point_datatype, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (mpi_comm_rank == 0) {
        if (check(res)) {
            std::cout << "Sort successful" << std::endl;
        } else {
            std::cout << "Sort error" << std::endl;
        }
    }
    MPI_Finalize();
	return 0;
}
