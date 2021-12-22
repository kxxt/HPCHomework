//
// Created by kxxt on 2021/12/11.
//

#include<cstdio>
#include<omp.h>

#pragma message("请使用 Intel C++ 编译器编译此程序!旧版的 g++ 不支持。")

int main() {
    int i, n = 2015, ic = 0;
#pragma omp parallel for default(none) shared(ic, n) private(i)
    for (i = 0; i < n; i++) {
#pragma omp atomic
        ic = 2 - ic;
    }
    printf("Counter=%d\n", ic);
    return 0;
}
