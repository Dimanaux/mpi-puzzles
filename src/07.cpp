#include <iostream>

#include "mpi.h"

#include "lib.h"

const int ARRAY_SIZE = 24;
const float A = 3.14;
const float B = 2.72;

int main(int argc, int **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    int chunk_size = ARRAY_SIZE / (size - 1);
    bool remains = (size - 1) * chunk_size == ARRAY_SIZE;

    if (rank == 0)
    {
        float *x = randomFloatArray(ARRAY_SIZE);
        float *y = randomFloatArray(ARRAY_SIZE);
        float *z = new float[ARRAY_SIZE];
        MPI_Status status;

        // a) z = Ax + By
        for (int i = 1; i < size; i++)
        {
            MPI_Send(x + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            MPI_Send(y + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(z + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }

        // b) y = Ax + By
        for (int i = 1; i < size; i++)
        {
            MPI_Send(x + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            MPI_Send(y + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(y + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }

        // c) z = x * y
        for (int i = 1; i < size; i++)
        {
            MPI_Send(x + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            MPI_Send(y + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(z + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }

        // d) x <-> y
        for (int i = 1; i < size; i++)
        {
            MPI_Send(x + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            MPI_Send(y + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(x + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
            MPI_Recv(y + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }
    }
    else if (size > 1)
    {
        float *x = new float[chunk_size];
        float *y = new float[chunk_size];
        float *z = new float[chunk_size];
        MPI_Status status;

        // a) z = Ax + By
        MPI_Recv(x, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Recv(y, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < chunk_size; i++)
        {
           z[i] = A * x[i] + B * y[i];
        }
        MPI_Send(z, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);

        // b) y = Ax + By
        MPI_Recv(x, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Recv(y, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < chunk_size; i++)
        {
           y[i] = A * x[i] + B * y[i];
        }
        MPI_Send(y, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);
 
        // c) z = x * y
        MPI_Recv(x, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Recv(y, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < chunk_size; i++)
        {
           z[i] = x[i] * y[i];
        }
        MPI_Send(z, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);

        // d) x <-> y
        MPI_Recv(x, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Recv(y, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < chunk_size; i++)
        {
            float tmp = x[i];
            x[i] = y[i];
            y[i] = tmp;
        }
        MPI_Send(x, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);
        MPI_Send(y, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

