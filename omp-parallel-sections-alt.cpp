//
// Created by kxxt on 2021/12/4.
//
#include "omp.h"
#include "cstdio"
#include "unistd.h"

int alpha() {
    usleep(1000000);
    printf("[Thread %d] Finished calculating alpha\n", omp_get_thread_num());
    return 1;
}

int beta() {
    usleep(3000000);
    printf("[Thread %d] Finished calculating beta\n", omp_get_thread_num());
    return 2;
}

int delta(int i, int j) {
    usleep(5000000);
    printf("[Thread %d] Finished calculating delta\n", omp_get_thread_num());
    return 3 * i + j;
}

int gamma(int i, int j) {
    usleep(20000);
    printf("[Thread %d] Finished calculating gamma\n", omp_get_thread_num());
    return i + 2 * j;
}

int epsilon(int x, int y) {
    usleep(20000);
    return x * y;
}

int main() {
    int i, j, k, w;
#pragma omp parallel default(none) shared(i, j, k, w)
    {
#pragma omp sections
        {
#pragma omp section
            i = alpha();
#pragma omp section
            j = beta();
        }
#pragma omp sections
        {
#pragma omp section
            k = delta(i, j);
#pragma omp section
            w = gamma(i, j);
        }
    }
    printf("%d\n", epsilon(k, w));
    return 0;
}