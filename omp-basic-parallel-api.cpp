//
// Created by kxxt on 2021/12/3.
//
#include "omp.h"
#include "cstdio"

int main() {
#pragma omp parallel default(none)
    {
        bool is_parallel = omp_in_parallel();
        printf("is_parallel: %d\n", is_parallel);
        printf("thread_num: %d\n", omp_get_num_threads());
        printf("thread_id: %d\n", omp_get_thread_num());
        printf("max_thread_num: %d\n", omp_get_max_threads());
        printf("num_proc: %d\n", omp_get_num_procs());
    }
    return 0;
}