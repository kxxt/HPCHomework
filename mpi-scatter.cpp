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
    int *data = new int[size], buf;
    if (rank == 0) {
        puts("Rank 0: Initializing data...\nThe data is shown as below:");
        for (int i = 0; i < size; i++) {
            data[i] = rand();
            printf("%.8x ", data[i]);
            if (i % 8 == 7) puts("");
        }
        if (size % 8 != 0) puts("");
        puts("Rank 0: Scattering data...");
    }
    MPI_Scatter(data, 1, MPI_INT, &buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Rank %d: Received 0x%.8x\n", rank, buf);
    // Finalize
    delete[] data;
    MPI_Finalize();
    return 0;
}