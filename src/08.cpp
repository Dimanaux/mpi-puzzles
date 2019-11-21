#include <iostream>

#include "mpi.h"

#include "lib.h"

const int MATRIX_SIZE = 24;

int main(int argc, int **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        // TODO
    }

    MPI_Finalize();
}

