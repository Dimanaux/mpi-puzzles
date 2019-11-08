#include <iostream>
#include <string>

#include "mpi.h"

#include "lib.h"

const int ARRAY_SIZE = 10;

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *a;
    if (rank == 0)
    {
        a = randomArray(ARRAY_SIZE);
        MPI_Send(a, ARRAY_SIZE, MPI_INT, 1, 99, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        a = new int[ARRAY_SIZE];
        MPI_Status status;
        MPI_Recv(a, ARRAY_SIZE, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        std::cout << "From process#" + std::to_string(rank) + ": " + toString(a, ARRAY_SIZE) + "\n";
    }
    MPI_Finalize();
}
