//
// Created by kxxt on 2021/11/27.
//

#include "mpi.h"
#include<cstdio>
#include<unistd.h>

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
    data = new int[datacnt];
    MPI_Allgatherv(buf, rank + 1, MPI_INT, data, recvcnts, displs, MPI_INT, MPI_COMM_WORLD);
    usleep(rank * 1000); // 防止输出错乱
    printf("Rank %d: gathered\n", rank);
    int cnt = 0;
    for (int i = 0; i < size; i++) {
        int color = i % 7 + 1;
        for (int j = 0; j < i + 1; j++, cnt++) {
            printf(COLOR_START "%d" COLOR_END "%.4d ", color, data[displs[i] + j]);
            if (cnt % 9 == 8)printf("\n");
        }
        printf(RESET);
    }
    puts("");
    delete[] data;
    // Finalize
    delete[] buf;
    MPI_Finalize();
    return 0;
}