//
// Created by kxxt on 2021/11/27.
//

#include "mpi.h"
#include <cstdio>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        MPI_Send(&rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    } else if (rank == 1) {
        MPI_Status status;
        int flag, cnt, tries = 0;
        do {
            printf("######## TEST %d ########\n", ++tries);
            MPI_Iprobe(0, 0, MPI_COMM_WORLD, &flag, &status);
            printf("flag=%d\n", flag);
            printf("source=%d\n", status.MPI_SOURCE);
            printf("tag=%d\n", status.MPI_TAG);
            printf("error=%d\n", status.MPI_ERROR);
            MPI_Get_count(&status, MPI_INT, &cnt);
            printf("count=%d\n", cnt);
        } while (!flag);
    }
    MPI_Finalize();
    return 0;
}
