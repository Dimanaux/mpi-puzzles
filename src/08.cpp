#include <iostream>

#include "mpi.h"

#include "lib.h"

const int MATRIX_ROWS = 6;
const int MATRIX_COLS = MATRIX_ROWS;
const int MATRIX_SIZE = MATRIX_ROWS * MATRIX_COLS;

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk_size = MATRIX_SIZE / (size - 1);
    MPI_Status status;

    if (rank == 0) {
        float** a = randomMatrix(MATRIX_ROWS, MATRIX_COLS);
        float** b = randomMatrix(MATRIX_ROWS, MATRIX_COLS);
        float** c = randomMatrix(MATRIX_ROWS, MATRIX_COLS);

        // a. c = a * b (C[i][j] = A[i][j] * B[i][j])
        for (int i = 1; i < size; i++) {
            MPI_Send(*a + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            MPI_Send(*b + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++) {
            MPI_Recv(*c + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }

        // b. c = a * b
        float** bt = transpose(b, MATRIX_ROWS, MATRIX_COLS);
        for (int i = 1; i < size; i++) {
            MPI_Send(*a + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            MPI_Send(*bt + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++) {
            MPI_Recv(*c + (i - 1) * chunk_size, chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }

        // c. transpose. Too difficult and meaningless
        b = transpose(a, MATRIX_ROWS, MATRIX_COLS);

        // d. Gauss method. Impossible
    } else {
        float* a = new float[chunk_size];
        float* b = new float[chunk_size];
        float* c = new float[chunk_size];

        // a.
        MPI_Recv(a, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Recv(b, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < chunk_size; i++) {
            c[i] = a[i] * b[i];
        }
        MPI_Send(c, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);

        // b.
        MPI_Recv(a, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Recv(b, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < chunk_size; i++) {
            c[i] = a[i] * b[i];
        }
        MPI_Send(c, chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);

        // c.

        // d.
    }

    MPI_Finalize();
}
