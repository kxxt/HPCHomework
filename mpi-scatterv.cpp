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
    int datacnt = (size + 1) * size >> 1;
    int *data, *buf = new int[rank + 1], sendcnts[size], displs[size];
    for (int i = 0; i < size; i++) {
        sendcnts[i] = i + 1;
        displs[i] = (i * (i + 1) >> 1);
    }
    if (rank == 0) {
        data = new int[datacnt];
        printf("Rank 0: datacnt=%d\n", datacnt);
        printf("Rank 0: sendcnts=");
        for (int i = 0; i < size; ++i) printf("%d ", sendcnts[i]);
        puts("");
        printf("Rank 0: displs=");
        for (int i = 0; i < size; ++i) printf("%d ", displs[i]);
        puts("");
        puts("Rank 0: Initializing data...\nThe data is shown as below:");
        for (int i = 0; i < datacnt; i++) {
            data[i] = rand();
            printf("%.8x ", data[i]);
            if (i % 8 == 7) puts("");
        }
        if (size % 8 != 0) puts("");
        puts("Rank 0: Scatterv data...");
    }
    MPI_Scatterv(data, sendcnts, displs, MPI_INT, buf, rank + 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Rank %d: Received ", rank);
    for (int i = 0; i < rank + 1; i++) printf("%.8x ", buf[i]);
    puts("");
    // MPI_Barrier(MPI_COMM_WORLD);
    // Finalize
    if (rank == 0)delete[] data;
    delete[] buf;
    MPI_Finalize();
    return 0;
}