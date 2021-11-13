#include<stdio.h>
#include"mpi.h"


inline void send_to_next(int current, int *value) {
    MPI_Send(value, 1, MPI_INT, current + 1, 0, MPI_COMM_WORLD);
    fprintf(stderr, "#%d read input %d\n", current, *value);
}

inline void recv_from_prev(int current, int *value, MPI_Status *status) {
    MPI_Recv(value, 1, MPI_INT, current - 1, 0, MPI_COMM_WORLD, status);
    fprintf(stderr, "#%d received %d from #%d\n", current, *value, current - 1);
}

int main(int argc, char **argv) {
    int myid, size, value;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int max_id = size - 1;
    do {
        if (myid == 0) {
            fprintf(stderr, "please input a value (input 0 to exit)->");
            scanf("%d", &value);
            send_to_next(myid, &value);
        } else if (myid != max_id) {
            recv_from_prev(myid, &value, &status);
            send_to_next(myid, &value);
        } else {
            recv_from_prev(myid, &value, &status);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    } while (value != 0);
    MPI_Finalize();
}



