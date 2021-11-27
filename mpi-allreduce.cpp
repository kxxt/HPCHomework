//
// Created by kxxt on 2021/11/27.
//

#include "mpi.h"
#include <cstdio>
#include <unistd.h>

// Ref : https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int result;
    unsigned char buf = (unsigned char) rank;
    MPI_Allreduce(&buf, &result, 1, MPI_BYTE, MPI_BXOR, MPI_COMM_WORLD);
    printf("Rank %.2d: Got reduced result " BYTE_TO_BINARY_PATTERN "\n", rank, BYTE_TO_BINARY(result));
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        usleep(1000);
        printf("Root process: Showing the mechanism.\n");
        for (int i = 0; i < size - 1; i++) {
            printf("\t" BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY((unsigned char) i));
        }
        printf("XOR\t" BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY((unsigned char) (size - 1)));
        printf("-----------------\n");
        printf("=\t" BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(result));
    }
    MPI_Finalize();
}