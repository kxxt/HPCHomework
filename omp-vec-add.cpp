//
// Created by kxxt on 2021/12/2.
//

#include<omp.h>
#include<cstdio>
#include <cstdlib>

int main() {
    int n = rand() % 10000;
    double *a = new double[n];
    double *b = new double[n];
    double *c = new double[n];
    for (int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = 3 * i + 2;
    }
    double start = omp_get_wtime();
#pragma omp parallel for default(none) shared(a, b, c, n)
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
    double end = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        printf("%f ", c[i]);
    }
    printf("\ntime: %f\n", end - start);
    delete[] a;
    delete[] b;
    delete[] c;
    return 0;
}