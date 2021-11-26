// MPI 代码模板
// Created by kxxt on 2011.11.22.

#include "mpi.h"
#include<cstdio>
#include<ctime>
#include <unistd.h>

const int MAX_CNT = 1000;

inline long get_exact_time(timespec t) {
    return t.tv_sec * 1000000 + t.tv_nsec;
}

#define LOG_TIME clock_gettime(CLOCK_MONOTONIC_RAW, &t);\
    printf("[%ld]", get_exact_time(t));

int main(int argc, char **argv) {
    // Initialize
    int rank, size;
    MPI_Request recv_req[MAX_CNT];
    timespec t; // 存储时间
    int result[MAX_CNT];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code
    MPI_Request req[MAX_CNT];
    for (int i = 0; i < size; i++) {
        LOG_TIME
        printf("#%d before async send to #%d\n", rank, i);
        if (rank == 2) usleep(100000);
        MPI_Isend(&rank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, req + i);
        LOG_TIME
        printf("#%d after async send to #%d\n", rank, i);
        MPI_Irecv(result + i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, recv_req + i);
    }
    int flag;
    MPI_Status recv_stat;
    for (int i = 0; i < size; i++) {
        // printf("rank=%d, src=%d, addr=%lX, req=%d\n", rank, i, long(recv_req + i), recv_req[i]);
        MPI_Test(recv_req + i, &flag, &recv_stat);
        if (flag) {
            LOG_TIME
            printf("#%d recv %d from #%d\n", rank, result[i], recv_stat.MPI_SOURCE);
        } else {
            LOG_TIME
            // 好像在接受完成之前，MPI_SOURCE 是没有赋值的， 因为我看到了如下的输出
            /*
                [3255456273941]#1 didn't finished recv from 4197952 0, Waiting...
                [3255456284305]#0 before async send to #2
                [3255456291278]#0 after async send to #2
                [3255456292601]#1 recv 0 from #0 after waiting
                [3255456307081]#0 recv 0 from #0
                [3255456310061]#1 recv 1 from #1
                [3255456323585]#0 recv 1 from #1
                [3255456325503]#1 didn't finished recv from 1 2, Waiting...
                [3255456339038]#0 didn't finished recv from 1 2, Waiting...
            */
            printf("#%d didn't finished recv from %d %d, Waiting...\n", rank, recv_stat.MPI_SOURCE, i);
            MPI_Wait(recv_req + i, &recv_stat);
            LOG_TIME
            printf("#%d recv %d from #%d after waiting\n", rank, result[i], recv_stat.MPI_SOURCE);
        }
    }
    for (int i = 0; i < size; i++) MPI_Wait(req + i, &recv_stat);
    // Finalize
    MPI_Finalize();
    return 0;
}