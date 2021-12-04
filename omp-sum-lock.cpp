//
// Created by kxxt on 2021/12/4.
//

#include "omp.h"
#include "cstdio"

int main() {
    int sum = 0, a[4] = {1, 2, 3, 4};
    omp_lock_t lock;
    omp_init_lock(&lock);
#pragma omp parallel default(none) shared(sum, a, lock)
    {
        int id = omp_get_thread_num(), local_sum = 0;
#pragma omp for
        for (int i = 0; i < 4; i++) {
            local_sum += a[i];
        }
        omp_set_lock(&lock);
        sum += local_sum;
        printf("Thread %d: local_sum = %d, sum = %d\n", id, local_sum, sum);
        omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
    return 0;
}
