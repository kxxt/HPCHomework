// Created by kxxt on 2011.11.13.

#include "mpi.h"
#include<cstdio>
#include<iostream>
#include<algorithm>

using namespace std;

#define MAX_BUFFER_WIDTH 1000
#define MAX_BUFFER_HEIGHT 1000

inline void get_demensions(int *m, int *n) {
    printf("Please input two integers (m and n, separated by space): ");
    scanf("%d %d", m, n);
}

inline void
get_mat_and_vec_from_input(int mat[MAX_BUFFER_HEIGHT * MAX_BUFFER_WIDTH], int vec[MAX_BUFFER_HEIGHT], int m, int n) {
    printf("Please input the matrix:\n");
    for (int i = 0; i < m * n; i++) scanf("%d", &mat[i]);
    printf("Please input the vector:\n");
    for (int i = 0; i < n; i++) scanf("%d", &vec[i]);
}

inline int inner_product(const int **arr1, const int **arr2, int l) {
    int sum = 0;
    for (int i = 0; i < l; i++) {
        sum += *arr1[i] * *arr2[i];
    }
    return sum;
}

inline void pretty_print_vec(const int *vec, int l) {
    putchar('[');
    putchar(' ');
    for (int i = 0; i < l; i++) {
        printf("%d ", vec[i]);
    }
    putchar(']');
    putchar('\n');
}

int main(int argc, char **argv) {
    // Initialize
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code
    int m, n, mat[MAX_BUFFER_HEIGHT * MAX_BUFFER_WIDTH], vec[MAX_BUFFER_HEIGHT];
    if (rank == 0) {
        get_demensions(&m, &n);
        get_mat_and_vec_from_input(mat, vec, m, n);
        int *tags = new int[size]; // 存储 tag 状态
        fill(tags, tags + size, 0); // 数组初始化为 0
        for (int i = 0; i < m; i++) {
            int tar = m % size;
            MPI_Send(&mat[n * i], n, MPI_INT, tar, tags[tar]++, MPI_COMM_WORLD);
        }
        delete[] tags;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vec, n, MPI_INT, 0, MPI_COMM_WORLD);
    pretty_print_vec(vec, n);
    int ntags = m / size + 1;
    // Finalize
    MPI_Finalize();
    return 0;
}