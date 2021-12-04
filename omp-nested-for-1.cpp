//
// Created by kxxt on 2021/12/3.
//

#include "omp.h"
#include "cstdio"

int main() {
    int i, j;
    double tstart = omp_get_wtime();
#pragma omp parallel for private(j)
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 6; j++) {
            printf("i=%d, j=%d\n", i, j);
        }
    }
    printf("time: %lf\n", omp_get_wtime() - tstart);
    return 0;
}