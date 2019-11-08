#include <stdio.h>

#include "mpi.h"

/**
 * Prints process number and processes amount.
 */
int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Process no. %d, MPI_COMM_WORLD size %d\n", rank, size);
    MPI_Finalize();
}
