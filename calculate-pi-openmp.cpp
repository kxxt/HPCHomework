//
// Created by kxxt on 2021/12/02.
//
#include<omp.h>
#include<cstdio>

#define NUM_THREADS 4

inline double f(double x) {
    //           4.0
    // f(x) = ---------
    //         1 + x^2
    // integrate f(x) = 4 arctan(x) + C
    // so integrate f(x) from 0 to 1 = pi
    return 4.0 / (1.0 + x * x);
}

int main() {
    // Initialize
    // Code
    int n;
    printf("number of small rectangles: ");
    scanf("%d", &n);
    double sum = 0.0;
    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        sum += f((i - 0.5) / n);
    }
    sum /= n;
    printf("pi = %.16lf\n", sum);
    // Finalize
    return 0;
}