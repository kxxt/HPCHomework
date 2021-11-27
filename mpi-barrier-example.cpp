#include "mpi.h"
#include<cstdio>

int main(int argc, char *argv[]) {
    int n_procs, proc_num, namelen;
    char pro_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Get_processor_name(pro_name, &namelen);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Hello World! Processor %d of %d on %s\n", proc_num, n_procs, pro_name);
    MPI_Finalize();
    return 0;
}