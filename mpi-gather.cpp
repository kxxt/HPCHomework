//
// Created by kxxt on 2021/11/27.
//

#include "mpi.h"
#include<cstdio>
#include<cstdlib>
#include<ctime>

int main(int argc, char **argv) {
    // Initialize
    srand(time(NULL));
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code
    int *data;
    if (rank == 0) data = new int[size];
    MPI_Gather(&rank, 1, MPI_INT, data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        puts("Root received: \n");
        for (int i = 0; i < size; i++) {
            printf("%.4d ", data[i]);
            if (i % 8 == 7) puts("");
        }
        if (size % 8 != 0) puts("");
        delete[] data;
    }
    // Finalize
    MPI_Finalize();
    return 0;
}