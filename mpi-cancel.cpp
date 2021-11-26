//
// Created by kxxt on 2021/11/26.
//


#include <mpi.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>

const int SIZE = 214748364;

inline long get_exact_time(timespec t) {
    return t.tv_sec * 1000000 + t.tv_nsec;
}

#define LOG_TIME clock_gettime(CLOCK_MONOTONIC_RAW, &t);\
    printf("[%ld]", get_exact_time(t));


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Status status;
    struct timespec t;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int buffer[SIZE];
    memset(buffer, 0, SIZE * sizeof(int));
    MPI_Request request;
    if (rank == 0) {
        // 根进程缓冲区全部初始化为 -1
        memset(buffer, 0xff, SIZE * sizeof(int));
        LOG_TIME
        printf("rank %d: before starting sending a large buffer.\n", rank);
        MPI_Isend(buffer, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        LOG_TIME
        printf("rank %d: after starting sending a large buffer.\n", rank);
        LOG_TIME
        printf("rank %d: before canceling the request.\n", rank);
        MPI_Cancel(&request);
        LOG_TIME
        printf("rank %d: after canceling the request.\n", rank);
        int flag;
        MPI_Wait(&request, &status);
        MPI_Test_cancelled(&status, &flag);
        if (flag) {
            LOG_TIME
            printf("rank %d: request was cancelled.\n", rank);
        } else {
            LOG_TIME
            printf("rank %d: request was not cancelled.\n", rank);
        }
    } else if (rank == 1) {
        LOG_TIME
        printf("rank %d: before receiving a large buffer.\n", rank);
        // usleep(100000);
        MPI_Recv(buffer, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        LOG_TIME
        printf("rank %d: after receiving a large buffer.\n", rank);
        printf("rank %d: buffer[0] = %d\n", rank, buffer[0]);
    }
    MPI_Finalize();
}
