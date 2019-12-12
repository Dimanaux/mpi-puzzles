#include <iostream>

#include "mpi.h"

#include "lib.h"

const int MATRIX_ROWS = 6;
const int MATRIX_COLS = MATRIX_ROWS;
const int MATRIX_SIZE = MATRIX_ROWS * MATRIX_COLS;
const int VECTOR_SIZE = MATRIX_ROWS;

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    int v_chunk_size = VECTOR_SIZE / (size - 1);
    int m_chunk_size = v_chunk_size * MATRIX_COLS;

    if (rank == 0) {
        float** matrix = randomMatrix(MATRIX_ROWS, MATRIX_COLS);
        float* vector = randomFloatArray(VECTOR_SIZE);

        // a.
        for (int i = 1; i < size; i++) {
            MPI_Send(*(matrix) + (i - 1) * m_chunk_size, m_chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }
        for (int i = 1; i < size; i++) {
            MPI_Recv(vector + (i - 1) * v_chunk_size, v_chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }
        logGot(rank, toString(vector, VECTOR_SIZE));

        // b.
        for (int i = 1; i < size; i++) {
            MPI_Recv(vector + (i - 1) * v_chunk_size, v_chunk_size, MPI_FLOAT, i, 99, MPI_COMM_WORLD, &status);
        }
        logGot(rank, toString(vector, VECTOR_SIZE));
    } else {
        float** m_chunk = newMatrix(v_chunk_size, MATRIX_COLS);
        float* v_chunk = new float[v_chunk_size];

        // a.
        MPI_Recv(*(m_chunk), m_chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < v_chunk_size; i++) {
            v_chunk[i] = m_chunk[i][i];
        }
        MPI_Send(v_chunk, v_chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);
        logSent(rank, toString(v_chunk, v_chunk_size));

        // b.
        for (int i = 0; i < v_chunk_size; i++) {
            int xi = 0;
            for (int j = 0; j < MATRIX_COLS; j++) {
                xi += v_chunk[i] * m_chunk[i][j];
            }
            v_chunk[i] = xi;
        }
        MPI_Send(v_chunk, v_chunk_size, MPI_FLOAT, 0, 99, MPI_COMM_WORLD);
        logSent(rank, toString(v_chunk, v_chunk_size));
    }

    MPI_Finalize();
}
