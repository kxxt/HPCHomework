//
// Created by kxxt on 2021/11/25.
//

#include "mpi.h"
#include<cstdio>

void custom_reduce_op(const int *invec, int *inoutvec, const int *len, MPI_Datatype *datatype) {
    int l = *len;
    for (int i = 0; i < l; i++) {
        inoutvec[i] *= 2 * invec[i];
    }
}

int main(int argc, char **argv) {
    // Initialize
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code
    MPI_Op custom_op;
    MPI_Op_create((void (*)(void *, void *, int *, MPI_Datatype *)) custom_reduce_op, true, &custom_op);
    int simd[8], ans[8];
    for (int i = 0; i < 8; i++)simd[i] = rank + i;
    MPI_Reduce(simd, ans, 8, MPI_INT, custom_op, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < 8; i++)printf("%d ", ans[i]);
        puts("");
    }
    // Finalize
    MPI_Op_free(&custom_op);
    MPI_Finalize();
    return 0;
}