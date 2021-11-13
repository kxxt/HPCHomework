#include<iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {
    int myid, size, value;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (myid == 0) {
        cout << "please input a value -> ";
        cin >> value;
        cout << "Process " << myid << " read from user " << value << endl;
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        cout << "Process " << myid << " send " << value << " to " << myid + 1 << endl;
    } else if (myid == 1) {
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        cout << "Process " << myid << " receive " << value << " from " << myid - 1 << endl;
    }

    MPI_Finalize();
}