//
// Created by kxxt on 2021/11/13.
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
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Code
    int n;
    if (0 == rank) {
        printf("number of small rectangles: ");
        scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double x, h, sum = 0.0;
    for (int i = rank + 1; i <= n; i += size) {
        sum += f((i - 0.5) / n);
    }
    sum /= n;
    // 上面的 for 循环根据 id 的不同异步执行不同部分的加和任务，都对n取平均（其实平均到最后再取也可以）
    // 加和任务分配(以5进程，n=7为例)：
    // id -> valid i
    // 0 -> 1, 6
    // 1 -> 2, 7
    // 2 -> 3
    // 3 -> 4
    // 4 -> 5
    // 下面通过 Reduce 将各个子进程的任务结果收集到 root 进程
    double pi;
    MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (0 == rank) {
        printf("Approximate value of pi: %.16f\n", pi);
        fflush(stdout);
    }
    // Finalize
    MPI_Finalize();
    return 0;
}