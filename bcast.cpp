#include "mpi.h"
#include <cstdio>
#include <ctime>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    struct timespec t; // 存储时间
    int rank, value = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    while (value > 0) {
        if (0 == rank) {
            clock_gettime(CLOCK_MONOTONIC_RAW, &t);
            cout << "[" << t.tv_sec * 1000000 + t.tv_nsec // 输出时间戳
                 << "] Please input a positive value, 0 to exit: \n";
            scanf("%d", &value);
        }
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        clock_gettime(CLOCK_MONOTONIC_RAW, &t);
        cout << "[" << t.tv_sec * 1000000 + t.tv_nsec << "] Process " << rank << " got value " << value << endl;
        // MPI_Barrier(MPI_COMM_WORLD);
        // 这里添加 Barrier 并不会让输出按顺序，
        // cout 的执行时间早的不一定先输出。
        // stdout is buffered on each processes.
    }
    MPI_Finalize();
    return 0;
}
