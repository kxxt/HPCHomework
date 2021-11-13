// Created by kxxt on 2011.11.13.

#include "mpi.h"
#include<cstdio>
#include<iostream>
#include<cstdlib>

using namespace std;

#define MAX_BUFFER_SIZE 1000000

inline void get_demensions(int *m, int *n, int *p) {
    printf("Please input three integers (m, n and p, separated by space): ");
    scanf("%d %d %d", m, n, p);
    if (*m * *n > MAX_BUFFER_SIZE || *n * *p > MAX_BUFFER_SIZE) {
        cerr << "Error: m*n and n*p should both be less than " << MAX_BUFFER_SIZE << endl;
        exit(1);
    }
}

inline void
get_lmat_and_rmat_from_input(int lmat[MAX_BUFFER_SIZE], int rmat[MAX_BUFFER_SIZE], int m,
                             int n, int p) {
    printf("Please input the matrix(m by n):\n");
    for (int i = 0; i < m * n; i++) scanf("%d", &lmat[i]);
    printf("Please input the matrix(n by p):\n");
    for (int i = 0; i < n * p; i++) scanf("%d", &rmat[i]);
}

inline int inner_product(const int *arr1, const int *arr2, int l) {
    // 向量内积
    int sum = 0;
    for (int i = 0; i < l; i++) sum += arr1[i] * arr2[i];
    return sum;
}

inline void pretty_print_mat(const int *mat, int m, int p) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            printf("%d", mat[p * i + j]);
            if (j != p - 1) putchar('\t');
        }
        if (i != m - 1)putchar('\n');
    }
}

void transpose(const int *mat, int *tmat, int m, int n) {
    // 转置矩阵，因为指针无法按列连续索引。
    for (int i = 0; i < m * n; i++) {
        int x = i / m; // row for n*m mat
        int y = i % m; // col for n*m mat
        tmat[i] = mat[y * n + x];
    }
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
    int m, n, p, lmat[MAX_BUFFER_SIZE], rmat[MAX_BUFFER_SIZE], trmat[MAX_BUFFER_SIZE];
    if (rank == 0) {
        // 获取用户输入
        get_demensions(&m, &n, &p);
        get_lmat_and_rmat_from_input(lmat, rmat, m, n, p);
        transpose(rmat, trmat, n, p); // 转置右矩阵
    }
    // 广播所有信息
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(trmat, n * p, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(lmat, n * m, MPI_INT, 0, MPI_COMM_WORLD);
    // pretty_print_mat(rmat, n, p);
    MPI_Request *requests = new MPI_Request[m * p / size + 1];
    int cur = 0;
    for (int i = rank; i <= m * p; i += size) {
        int row = i / m;
        int col = i % m;
        int prod = inner_product(&lmat[row * n], &trmat[col * n], n);
        MPI_Isend(&prod, 1, MPI_INT, 0, i, MPI_COMM_WORLD, &requests[cur++]); // 异步发送，不阻塞循环
    }
    // pretty_print_mat(lmat, m, p);
    if (rank == 0) {
        int result[MAX_BUFFER_SIZE];
        for (int i = 0; i < m * p; i++) {
            // cout << "recv " << i << endl;
            MPI_Recv(&result[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
        }
        cout << "Result：" << endl;
        pretty_print_mat(result, m, p);
        putchar('\n');
    }
    // 等待所有发送结束
    for (int i = 0; i < cur; i++)MPI_Wait(requests + i, &status);
    // Finalize
    delete[] requests; // 释放资源
    MPI_Finalize();
    return 0;
}