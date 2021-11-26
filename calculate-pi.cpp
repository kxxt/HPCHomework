//
// Created by kxxt on 2021/11/26.
//
#include "mpi.h"
#include<cstdio>

inline double f(double x) {
    //           4.0
    // f(x) = ---------
    //         1 + x^2
    // integrate f(x) = 4 arctan(x) + C
    // so integrate f(x) from 0 to 1 = pi
    return 4.0 / (1.0 + x * x);
}

int main(int argc, char **argv) {
    // Initialize
    int rank, size;
    double start_time, end_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Code
    int n;
    if (0 == rank) {
        printf("number of small rectangles: ");
        scanf("%d", &n);
    }
    start_time = MPI_Wtime();
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double x, h, sum = 0.0;
    for (int i = rank + 1; i <= n; i += size) {
        sum += f((i - 0.5) / n);
    }
    sum /= n;
    double pi;
    MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    if (0 == rank) {
        printf("Approximate value of pi: %.16f\nTime used: %f seconds\n", pi, end_time - start_time);
    }
    // Finalize
    MPI_Finalize();
    return 0;
}