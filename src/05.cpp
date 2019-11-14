#include <iostream>

#include "mpi.h"

#include "lib.h"

const int ARRAY_SIZE = 12;

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk_size = ARRAY_SIZE / (size + 1);

    if (rank == 0)
    {
        std::cout << "Chunk size: " + std::to_string(chunk_size) + "\n";
        int *array = randomArray(ARRAY_SIZE);
        for (int i = 1; i < size; i++)
        {
            MPI_Send(array + (i - 1) * chunk_size, chunk_size, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
        std::cout << "Process#" + std::to_string(rank) + " sent: " + toString(array, ARRAY_SIZE) + "\n";
    }
    else
    {
        int *local_chunk = new int[chunk_size];
        MPI_Status status;
        MPI_Recv(local_chunk, chunk_size, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        std::cout << "Process#" + std::to_string(rank) + " got: " + toString(local_chunk, chunk_size) + "\n";
    }

    MPI_Finalize();
}
