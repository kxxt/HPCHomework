// Created by kxxt on 2011.11.13.

#include "mpi.h"
#include<cstdio>
#include<iostream>
#include<cstdlib>

using namespace std;

#define MAX_BUFFER_WIDTH 1000
#define MAX_BUFFER_HEIGHT 1000

inline void get_demensions(int *m, int *n) {
    printf("Please input two integers (m and n, separated by space): ");
    scanf("%d %d", m, n);
    if (*m > MAX_BUFFER_HEIGHT || *n > MAX_BUFFER_WIDTH) {
        cerr << "Error: m should be less than " << MAX_BUFFER_HEIGHT << " and n should be less than "
             << MAX_BUFFER_WIDTH << endl;
        exit(1);
    }
}

inline void
get_mat_and_vec_from_input(int mat[MAX_BUFFER_HEIGHT * MAX_BUFFER_WIDTH], int vec[MAX_BUFFER_HEIGHT], int m, int n) {
    printf("Please input the matrix:\n");
    for (int i = 0; i < m * n; i++) scanf("%d", &mat[i]);
    printf("Please input the vector:\n");
    for (int i = 0; i < n; i++) scanf("%d", &vec[i]);
}

inline int inner_product(const int *arr1, const int *arr2, int l) {
    int sum = 0;
    for (int i = 0; i < l; i++) sum += arr1[i] * arr2[i];
    return sum;
}

inline void pretty_print_vec(const int *vec, int l) {
    cout << "[";
    for (int i = 0; i < l; i++) {
        printf("%d", vec[i]);
        if (i != l - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main(int argc, char **argv) {
    // Initialize
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code
    // 这里我直接像 OI 里常做的那样把数组大小写死了，当然也可以动态申请内存。
    int m, n, mat[MAX_BUFFER_HEIGHT * MAX_BUFFER_WIDTH], vec[MAX_BUFFER_HEIGHT];
    if (rank == 0) {
        // 获取用户输入
        get_demensions(&m, &n);
        get_mat_and_vec_from_input(mat, vec, m, n);
    }
    // 广播所有信息
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vec, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(mat, n * m, MPI_INT, 0, MPI_COMM_WORLD);
    // pretty_print_vec(vec, n);
    MPI_Request *requests = new MPI_Request[m / size + 1];
    int cur = 0;
    for (int i = rank; i <= m; i += size) {
        // cout << i << endl;
        int prod = inner_product(&mat[i * m], vec, n);
        MPI_Isend(&prod, 1, MPI_INT, 0, i, MPI_COMM_WORLD, &requests[cur++]); // 异步发送，不阻塞循环
    }
    if (rank == 0) {
        int result[MAX_BUFFER_HEIGHT];
        for (int i = 0; i < m; i++) {
            // cout << "recv " << i << endl;
            MPI_Recv(&result[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
        }
        pretty_print_vec(result, n);
    }
    // 等待所有发送结束
    for (int i = 0; i < cur; i++)MPI_Wait(requests + i, &status);
    // Finalize
    delete[] requests; // 释放资源
    MPI_Finalize();
    return 0;
}