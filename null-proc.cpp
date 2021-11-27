//
// Created by kxxt on 2021/11/27.
//

#include "mpi.h"
#include <cstdio>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, buf;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int receiver = rank < size - 1 ? rank + 1 : MPI_PROC_NULL;
    int sender = rank > 0 ? rank - 1 : MPI_PROC_NULL;
    MPI_Send(&rank, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);
    printf("%d sended %d to %d\n", rank, rank, receiver);
    MPI_Recv(&buf, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d received %d from %d\n", rank, buf, sender);
    MPI_Finalize();
    return 0;
}