//
// Created by kxxt on 2021/11/26.
//

#include "mpi.h"
#include <cstdio>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int buf;
    if (rank == 0) {
        MPI_Sendrecv(&rank, 1, MPI_INT, 1, 0,
                     &buf, 1, MPI_INT, 1, 0,
                     MPI_COMM_WORLD, &status);
        printf("#%d recieved %d\n", rank, buf);
    } else if (rank == 1) {
        MPI_Sendrecv(&rank, 1, MPI_INT, 0, 0,
                     &buf, 1, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, &status);
        printf("#%d recieved %d\n", rank, buf);
    }
    MPI_Finalize();
}