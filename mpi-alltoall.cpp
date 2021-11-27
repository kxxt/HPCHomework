//
// Created by kxxt on 2021/11/27.
//

#include "mpi.h"
#include <cstdio>
#include <unistd.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int sendbuf[size], recvbuf[size];
    for (int i = 0; i < size; i++) sendbuf[i] = rank * size + i;
    usleep(rank * 1000);
    printf("rank %d: Data:\t", rank);
    for (int i = 0; i < size; i++) printf("%.4d ", sendbuf[i]);
    puts("");
    MPI_Alltoall(sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, MPI_COMM_WORLD);
    usleep(rank * 1000);
    printf("rank %d: Received:\t", rank);
    for (int i = 0; i < size; i++) printf("%.4d ", recvbuf[i]);
    puts("");
    MPI_Finalize();
}