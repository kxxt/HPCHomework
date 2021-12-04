//
// Created by kxxt on 2021/12/4.
//
#include <omp.h>
#include <cstdio>

int main() {
#pragma omp parallel for schedule(static, 3) default(none)
    for (int i = 0; i < 10; i++) {
        printf("thread %d, iteration %d\n", omp_get_thread_num(), i);
    }
    return 0;
}