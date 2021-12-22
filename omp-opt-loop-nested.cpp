//
// Created by kxxt on 2021/12/11.
//

#include <omp.h>
#include <cstdio>

const int N = 100000;
const int M = 10000;
const int OMP_NUM_THREADS = 8;

double mat[N*M];

int main() {
    printf("Set OMP_NUM_THREADS=%d\n", OMP_NUM_THREADS);
    omp_set_num_threads(OMP_NUM_THREADS);
    double start = omp_get_wtime();
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
#pragma omp parallel for
        for (int j = 0; j < M; ++j) {
            mat[i*M+j] = i % 10086;
        }
    }
    printf("Time: %f\n", omp_get_wtime() - start);
}
