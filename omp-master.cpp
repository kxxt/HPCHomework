//
// Created by kxxt on 2021/12/4.
//

#include "omp.h"
#include <cstdio>

int main() {
    int b[6], a;
#pragma omp parallel default(none) shared(a, b)
    {
        int tid = omp_get_thread_num();
#pragma omp master
        {
            a = 10;
            printf("Master thread %d executed master block, a=%d\n", tid, a);
        }
#pragma omp barrier
#pragma omp for
        for (int i = 0; i < 6; ++i) {
            b[i] = tid + a;
        }
    }
    for (int i = 0; i < 6; ++i) {
        printf("b[%d] = %d\n", i, b[i]);
    }
    return 0;
}