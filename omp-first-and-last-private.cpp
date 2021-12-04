//
// Created by kxxt on 2021/12/3.
//

#include <cstdlib>
#include "omp.h"
#include "cstdio"

int main() {
    int val = 8;
    printf("[%lf]Before parallel: val=%d\n", omp_get_wtime(), val);
#pragma omp parallel for default(none) firstprivate(val) lastprivate(val)
    for (int i = 0; i < 4; i++) {
        int TID = omp_get_thread_num(), r = rand();
        printf("[%lf]Thread %d, iteration %d: before changing val to %d, val=%d\n", omp_get_wtime(), TID, i, r, val);
        val = r;
        printf("[%lf]Thread %d, iteration %d: after changing val to %d, val=%d\n", omp_get_wtime(), TID, i, r, val);
    }
    fprintf(stderr, "注意：由于 stdout 存在缓冲区，输出顺序并不代表线程的执行顺序，线程的执行顺序可以从前面的时间看出。"
                    "线程的退出顺序我无法得到，所以输出时间最晚的线程不一定是最后退出并行区的线程。\n"
                    "要将输出按照时间顺序显示，请把程序输出 pipe 到 sort 命令。比如一下命令：\n"
                    "\x1B[32m$ mpcxxrunner omp-first-and-last-private 3 | sort\x1B[0m\n");
    printf("[%lf]After parallel: val=%d\n", omp_get_wtime(), val);
    return 0;
}