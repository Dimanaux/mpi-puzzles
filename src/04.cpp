#include <iostream>
#include <string>

#include "mpi.h"

#include "lib.h"

const int ARRAY_SIZE = 12;

int main(int argc, char **argv)
{

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *array;

    if (rank == 1)
    {
        array = randomArray(ARRAY_SIZE);
        for (int i = 0; i < size; i++)
        {
            if (i == rank)
                continue;
            MPI_Send(array, ARRAY_SIZE, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    }
    else if (size > 1)
    {
        MPI_Status status;
        int s = MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (s == MPI_SUCCESS && status.MPI_SOURCE == 1 && status.MPI_TAG == 99)
        {
            array = new int[ARRAY_SIZE];
            MPI_Recv(array, ARRAY_SIZE, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
            std::cout << "Process#" + std::to_string(rank) + " received: " + toString(array, ARRAY_SIZE) + "\n";
        }
    }

    MPI_Finalize();
}
