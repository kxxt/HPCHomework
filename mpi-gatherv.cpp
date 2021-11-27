//
// Created by kxxt on 2021/11/27.
//

#include "mpi.h"
#include<cstdio>

#define COLOR_START "\x1B[3"
#define COLOR_END   "m"
#define RESET "\x1B[0m"


int main(int argc, char **argv) {
    // Initialize
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code
    int datacnt = (size + 1) * size >> 1;
    int *data, *buf = new int[rank + 1], recvcnts[size], displs[size];
    for (int i = 0; i < size; i++) {
        recvcnts[i] = i + 1;
        displs[i] = (i * (i + 1) >> 1);
    }
    for (int i = 0; i < rank + 1; i++)buf[i] = i;
    if (rank == 0) data = new int[datacnt];
    MPI_Gatherv(buf, rank + 1, MPI_INT, data, recvcnts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Rank %d: Received\n", rank);
        int cnt = 0;
        for (int i = 0; i < size; i++) {
            printf(COLOR_START "%d" COLOR_END, i % 7 + 1);
            for (int j = 0; j < i + 1; j++, cnt++) {
                printf("%.4d ", data[displs[i] + j]);
                if (cnt % 9 == 8)printf("\n");
            }
            printf(RESET);
        }
//        for (int i = 0; i < datacnt; i++) {
//            printf("%.4d ", data[i]);
//            if (i % 8 == 7)printf("\n");
//        }
        if (datacnt % 9 != 0)printf("\n");
        delete[] data;
    }
    // MPI_Barrier(MPI_COMM_WORLD);
    // Finalize
    delete[] buf;
    MPI_Finalize();
    return 0;
}