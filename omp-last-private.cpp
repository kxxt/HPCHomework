//
// Created by kxxt on 2021/12/3.
//

#include "omp.h"
#include "cstdio"

int main() {
    int val = 8;
    printf("Before parallel: val=%d\n", val);
#pragma omp parallel for lastprivate(val)
    for (int i = 0; i < 4; i++) {
        int TID = omp_get_thread_num();
        printf("Thread %d, iteration %d: before changing val to %d, val=%d\n", TID, i, i, val);
        val = i;
        printf("Thread %d, iteration %d: after changing val to %d, val=%d\n", TID, i, i, val);
    }
    printf("After parallel: val=%d\n", val);
    return 0;
}