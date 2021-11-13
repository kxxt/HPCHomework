//
// Created by kxxt on 2021/11/13.
//

#include "mpi.h"
#include<cstdio>

int main(int argc, char **argv) {
    int id, size, buf;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (id == 0) {
        // Setup: as a receiver
        for (int i = 0; i < 10 * (size - 1); i++) {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            fprintf(stderr, "[INFO#%d] %d, with tag %d\n", status.MPI_SOURCE, buf, status.MPI_TAG);
        }
    } else {
        for (int i=0;i<10;i++){
            buf = 10 * id + i;
            MPI_Send(&buf, 1, MPI_INT, 0, i, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
}