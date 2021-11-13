#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int rank, value = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    while (value > 0) {
        if (0 == rank) {
            printf("Please input a positive value: 0 to exit\n");
            scanf("%d", &value);
        }
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Process %d got value %d\n", rank, value);
    }
    MPI_Finalize();
    return 0;
}
