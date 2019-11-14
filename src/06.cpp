#include <iostream>

#include "mpi.h"

#include "lib.h"

const int VECTOR_SIZE = 24;

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk_size = VECTOR_SIZE / (size - 1);

    if (rank == 0)
    {
        int *x = randomArray(VECTOR_SIZE);
        int *y = randomArray(VECTOR_SIZE);

        logGot(rank, toString(x, VECTOR_SIZE));
        logGot(rank, toString(y, VECTOR_SIZE));

        MPI_Status status;

        for (int i = 1; i < size; i++)
        {
            MPI_Send(x + (i - 1) * chunk_size, chunk_size, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Send(y + (i - 1) * chunk_size, chunk_size, MPI_INT, i, 99, MPI_COMM_WORLD);
        }

        int *z = new int[VECTOR_SIZE];

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(z + (i - 1) * chunk_size, chunk_size, MPI_INT, i, 99, MPI_COMM_WORLD, &status);
        }

        logGot(rank, toString(z, VECTOR_SIZE));
    }
    else
    {
        MPI_Status status;
        int* x_chunk = new int[chunk_size];
        int* y_chunk = new int[chunk_size];
        MPI_Recv(x_chunk, chunk_size, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Recv(y_chunk, chunk_size, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        for (int i = 0; i < chunk_size; i++)
        {
            x_chunk[i] *= y_chunk[i];
        }
        MPI_Send(x_chunk, chunk_size, MPI_INT, 0, 99, MPI_COMM_WORLD);
        logSent(rank, toString(x_chunk, chunk_size));
    }

    MPI_Finalize();
}