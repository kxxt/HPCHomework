// MPI 代码模板
// Created by kxxt on 2011.11.13.

#include "mpi.h"
#include<cstdio>

int main(int argc, char **argv) {
    // Initialize
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code

    // Finalize
    MPI_Finalize();
    return 0;
}