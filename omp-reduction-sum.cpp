//
// Created by kxxt on 2021/12/4.
//

#include "omp.h"
#include <cstdio>

int main() {
    int sum = 0;
#pragma omp parallel for num_threads(10) default(none) reduction(+:sum)
    for (int i = 1; i <= 1000000; i++) {
        sum += i;
    }
    printf("sum = %d\n", sum);
    return 0;
}