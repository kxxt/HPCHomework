//
// Created by kxxt on 2021/12/4.
//

#include "omp.h"
#include <cstdio>
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

int delta() {
    usleep(5000000);
    printf("Finished calculating delta\n");
    return 3;
}

int omicron(int i, int j, int k) {
    usleep(70000);
    printf("Finished calculating omicron\n");
    return i + j + k;
}

int main() {
    int i, j, k;
#pragma omp parallel default(none) shared(i, j, k)
    {
        int id = omp_get_thread_num();
        if (id == 0) {
            i = alpha();
        } else if (id == 1) {
            j = beta();
        } else if (id == 2) {
            k = delta();
        }
    }
    printf("%d\n", omicron(i, j, k));
    return 0;
}