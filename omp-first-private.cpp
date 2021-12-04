//
// Created by kxxt on 2021/12/3.
//

#include "omp.h"
#include "cstdio"

int main() {
    int val = 8;
    printf("Before parallel: val=%d\n", val);
#pragma omp parallel default(none) firstprivate(val)
    {
        int TID = omp_get_thread_num();
        printf("Thread %d before changing val to 23, val=%d\n", TID, val);
        val = 23;
        printf("Thread %d after changing val to 23, val=%d\n", TID, val);
    }
    printf("After parallel: val=%d\n", val);
    return 0;
}