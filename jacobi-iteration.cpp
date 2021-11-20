// Created by kxxt on 2011.11.13.

#include "mpi.h"
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<iomanip>

using namespace std;

#define MAX_BUFFER_SIZE 1000000

// ref: https://stackoverflow.com/questions/9158150/colored-output-in-c
#define YELLOW      "\033[33m"
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define BOLDWHITE   "\033[1m\033[37m"

inline void get_demensions(int *m, int *n) {
    printf("Please input two integers (\033[36mm\033[0m and \033[36mn\033[0m, separated by \033[36mspace\033[0m): ");
    scanf("%d %d", m, n);
    if (*m * *n > MAX_BUFFER_SIZE) {
        cerr << RED << "Error: m*n should be less than " << MAX_BUFFER_SIZE << RESET << endl;
        exit(1);
    }
}

const int CHUNK_WIDTH_TOO_SMALL = -1;
const int CHUNK_HEIGHT_TOO_SMALL = -2;

inline int get_chunk_size(int m, int n) {
    // 获取适用于 m x n 矩阵的合理的分块高度
    if (m < 3) return CHUNK_HEIGHT_TOO_SMALL;
    if (n < 3) return CHUNK_WIDTH_TOO_SMALL;
    if (m == 3) return 3;
    if (m <= 10) return m / 2;
    return 4;
}

inline void get_mat_from_input(double lmat[MAX_BUFFER_SIZE], int m, int n) {
    printf("Please input the matrix(m by n):\n");
    for (int i = 0; i < m * n; i++) scanf("%lf", &lmat[i]);
}

inline void pretty_print_mat(const double *mat, int m, int p) {
    std::cout << std::fixed;
    std::cout << std::setprecision(4);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            // 边界值加粗输出
            if (i == 0 || i == m - 1 || j == 0 || j == p - 1) cout << BOLDWHITE << mat[p * i + j] << RESET;
            else cout << mat[p * i + j];
            if (j != p - 1) putchar('\t');
        }
        putchar('\n');
    }
}

const double EPSILON = 1e-2; // 收敛标准

bool test_convergence(const double *oldmat, const double *newmat, int m, int n) {
    double diff = 0.0;
    int len = m * n;
    for (int i = 0; i < len; i++) diff += (oldmat[i] - newmat[i]) * (oldmat[i] - newmat[i]);
    return diff < EPSILON * EPSILON;
}

int adjust_chunk_size_and_emit_warnings(int chunk_size, int m, int size);

// Message types for `dispatch`
typedef int MessageType;
const int MSG_EXIT = 1;
const int MSG_TERMINATE_UNUSED = 2; // With argument: new size
const int MSG_COMPUTE = 3; // With argument: chunk size
const int MSG_SEND_N = 4;
const int MSG_SEND_M = 5;

void dispatch(const int *value, MessageType type, int size); // 命令调度函数

inline void compute(const double *src, double *to, int rank, int chunk_size, int size, int m, int n);

int main(int argc, char **argv) {
    // Initialize
    int rank, size, new_size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Code
    // 这里我直接像 OI 里常做的那样把数组大小写死了，当然也可以动态申请内存。
    int m, n, chunk_size;
    const int nullmsg = 0;
    double mat[MAX_BUFFER_SIZE], nmat[MAX_BUFFER_SIZE], *pmat = mat, *pnmat = nmat;
    if (rank == 0) {
        // 获取用户输入
        if (size == 1) {
            cerr << RED << "[ERROR] At least 2 processes are required!" << RESET << endl;
            exit(1);
        }
        get_demensions(&m, &n);
        get_mat_from_input(mat, m, n);
        chunk_size = get_chunk_size(m, n);
        if (chunk_size == CHUNK_HEIGHT_TOO_SMALL || chunk_size == CHUNK_WIDTH_TOO_SMALL) {
            // 矩阵太小直接输出
            pretty_print_mat(mat, m, n);
            dispatch(&nullmsg, MSG_EXIT, size);
            // cout << "Sent Exit!" << endl;
        } else {
            // 广播所有信息
            chunk_size = adjust_chunk_size_and_emit_warnings(chunk_size, m, size);
            new_size = m / chunk_size + 1;
            dispatch(&new_size, MSG_TERMINATE_UNUSED, size);
            dispatch(&m, MSG_SEND_M, new_size);
            dispatch(&n, MSG_SEND_N, new_size);
            // cout << "Cleaned unused processes." << endl;
            int iteration = 0;
            do {
                dispatch(&chunk_size, MSG_COMPUTE, new_size);
                for (int i = 1; i < new_size; i++)
                    MPI_Send(pmat, m * n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                for (int i = 1; i < new_size - 1; i++)
                    MPI_Recv(pnmat + (i - 1) * n * chunk_size, n * chunk_size, MPI_DOUBLE, i, MPI_ANY_TAG,
                             MPI_COMM_WORLD, &status);
                MPI_Recv(pnmat + (new_size - 2) * n * chunk_size,
                         n * (chunk_size + m % chunk_size), // 最后一块大小不一样需要单独处理。
                         MPI_DOUBLE, new_size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                // BEGIN 复制边界到新数组
                memcpy(pnmat, pmat, n * sizeof(double));
                memcpy(pnmat + (m - 1) * n, pmat + (m - 1) * n, n * sizeof(double));
                for (int i = 1; i < m - 1; i++) {
                    pnmat[i * n] = pmat[i * n];
                    pnmat[i * n + n - 1] = pmat[i * n + n - 1];
                }
                // END 复制边界到新数组
                // 输出每次迭代的结果
                cout << GREEN << "Iteration " << ++iteration << RESET << endl;
                pretty_print_mat(pnmat, m, n);
                // 交换新旧数组指针
                double *tmp = pmat;
                pmat = pnmat;
                pnmat = tmp;
            } while (not test_convergence(pmat, pnmat, m, n));
            // pretty_print_mat(pmat, m, n); // 输出 pmat, 因为交换过两个矩阵指针
            dispatch(&nullmsg, MSG_EXIT, new_size);
        }

    } else {
        int buf;
        bool exit = false;
        while (not exit) {
            MPI_Recv(&buf, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            switch (status.MPI_TAG) {
                case MSG_EXIT:
                    exit = true;
                    break;
                case MSG_SEND_M:
                    m = buf;
                    break;
                case MSG_SEND_N:
                    n = buf;
                    break;
                case MSG_TERMINATE_UNUSED:
                    size = buf; // 调整 size 大小
                    if (rank >= size) exit = true;
                    break;
                case MSG_COMPUTE:
                    // cout << "#" << rank << " received MSG_COMPUTE!" << endl;
                    chunk_size = buf;
                    MPI_Recv(mat, m * n, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    compute(mat, nmat, rank, chunk_size, size, m, n);
                    MPI_Send(nmat + (rank - 1) * n * chunk_size,
                             rank == new_size - 1 ? n * (chunk_size + m % chunk_size) : n * chunk_size,
                             MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                    break;
                default:
                    cerr << "[ERROR #" << rank << "] Unknown Message Received: " << status.MPI_TAG << endl;
                    break;
            }
        }
    }

    // Finalize
    MPI_Finalize();
    return 0;
}

inline void compute(const double *src, double *to, int rank, int chunk_size, int size, int m, int n) {
    // 区间左闭右开
    int rstart = (rank - 1) * chunk_size, rend = rank * chunk_size;
    if (rank == 1) rstart = 1;
    if (rank == size - 1) rend = m - 1;
    const int cstart = 1, cend = n - 1;
    // 输出计算区间
    // cout << "#" << rank << " @[" << rstart << ", " << rend << "]x[" << cstart << ", " << cend << ']' << endl;
    for (int i = rstart; i < rend; i++)
        for (int j = cstart; j < cend; j++)
            to[i * n + j] = (src[(i - 1) * n + j] + src[(i + 1) * n + j] + src[i * n + j - 1] + src[i * n + j + 1]) / 4;
//    // 输出第一个进程的计算结果
//    if (rank == 1) {
//        cout << "Compute @ #1" << endl;
//        pretty_print_mat(to, m, n);
//        cout << endl;
//    }
}

int adjust_chunk_size_and_emit_warnings(int chunk_size, int m, int size) {
    int nrequired = m / chunk_size + 1;
    if (nrequired < size) {
        cout << YELLOW << "[WARNING]: I will only use " << nrequired
             << " processes. Other processes will be terminated soon." << RESET << endl;
    } else if (nrequired > size) {
        int capicity = size - 1;
        int chunk_new_size = m / capicity;
        cout << YELLOW << "[WARNING]: chunk size will be adjusted to " << chunk_new_size << " due to too few processes."
             << RESET << endl;
        return chunk_new_size;
    }
    return chunk_size;
}


void dispatch(const int *value, MessageType type, int size) {
    for (int i = 1; i < size; i++) {
        MPI_Send(value, 1, MPI_INT, i, type, MPI_COMM_WORLD);
    }
}