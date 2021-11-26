//
// Created by kxxt on 2021/11/26.
//

#include "mpi.h"
#include <cstdio>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    puts("This code won't run. It's an example of deadlock!");
    int info1 = 23, info2 = 10;
    if (rank == 0) {
        MPI_Send(&info1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&info2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    } else if (rank == 1) {
        MPI_Send(&info2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&info1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Finalize();
    return 0;
}