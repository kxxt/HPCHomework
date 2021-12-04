//
// Created by kxxt on 2021/12/2.
//
#include <omp.h>
#include <cstdio>

int main() {
#pragma omp parallel
    {
        int ID=omp_get_thread_num();
        printf("Hello World from %d!\n", ID);
    }
    return 0;
}
