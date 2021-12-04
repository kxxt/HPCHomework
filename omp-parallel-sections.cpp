//
// Created by kxxt on 2021/12/4.
//
#include "omp.h"
#include "cstdio"
#include "unistd.h"

int alpha() {
    usleep(1000000);
    printf("Finished calculating alpha\n");
    return 1;
}

int beta() {
    usleep(3000000);
    printf("Finished calculating beta\n");
    return 2;
}

int delta() {
    usleep(5000000);
    printf("Finished calculating delta\n");
    return 3;
}

int gamma(int i, int j, int k) {
    usleep(20000);
    return i + j + k;
}

int epsilon(int x, int y) {
    usleep(20000);
    return x * y;
}

int main() {
    int i, j, k;
#pragma omp parallel sections default(none) shared(i, j, k)
    {
#pragma omp section
        i = alpha();
#pragma omp section
        j = beta();
#pragma omp section
        k = delta();
    }
    int result = gamma(i, j, k);
    printf("%d\n", epsilon(result, i));
    return 0;
}