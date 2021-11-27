//
// Created by kxxt on 2021/11/27.
//

#include <mpi.h>
#include<cstdio>
#include<ctime>
#include<cstdlib>

const int BUFFER_SIZE = 1024;

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int cnt;
    MPI_Status status;
    int data[BUFFER_SIZE];
    if (rank == 0) {
        srand((unsigned int) time(NULL));
        cnt = rand() % BUFFER_SIZE;
        for (int i = 0; i < cnt; i++) {
            data[i] = rand();
        }
        MPI_Send(&data, cnt, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("[MPI Probe #%d] status.MPI_SOURCE=%d, status.MPI_TAG=%d, status.MPI_ERROR=%d.\n", rank,
               status.MPI_SOURCE,
               status.MPI_TAG, status.MPI_ERROR);
        MPI_Get_count(&status, MPI_INT, &cnt);
        printf("[Got Length#%d] message length=%d.\n", rank, cnt);
        MPI_Recv(&data, cnt, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        puts("--------- BEGIN RECEIVED BUFFER ---------");
        for (int i = 0; i < cnt; i++) {
            printf("%.8X ", data[i]);
            if (i % 8 == 7)puts("");
        }
        if (cnt % 8 != 0)puts("");
        puts("--------- END  RECEIVED BUFFER ---------");
    }
    MPI_Finalize();
    return 0;
}
