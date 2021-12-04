//
// Created by kxxt on 2021/12/4.
//

#include "omp.h"
#include <cstdio>
#include "unistd.h"

inline void print_time() {
    printf("[%lf]", omp_get_wtime());
}

int main() {
#pragma omp parallel num_threads(4) default(none)
    {
        int tid = omp_get_thread_num();
        int sleep = tid * 100000;
        print_time();
        printf("Thread %d before sleeping %d us\n", tid, sleep);
        usleep(sleep);
        print_time();
        printf("Thread %d after sleeping %d us\n", tid, sleep);
        print_time();
        printf("thread %d before barrier\n", tid);
#pragma omp barrier
        print_time();
        printf("thread %d after barrier\n", tid);
    }
    return 0;
}