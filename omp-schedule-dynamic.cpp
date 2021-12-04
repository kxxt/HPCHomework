//
// Created by kxxt on 2021/12/4.
//
#include <omp.h>
#include <cstdio>

int main() {
    printf("分为五个任务，只要有任务并且线程空闲，任务就会分配给空闲线程\n");
#pragma omp parallel for schedule(dynamic, 2) default(none)
    for (int i = 0; i < 10; i++) {
        printf("thread %d, iteration %d\n", omp_get_thread_num(), i);
    }
    return 0;
}