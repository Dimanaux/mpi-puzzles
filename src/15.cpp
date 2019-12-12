#include <iostream>

#include "mpi.h"

#include "lib.h"

using namespace std;

const int ROWS = 4;
const int COLS = 4;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    float** a;
    float** b;
    float** c;

    if (rank == 0) {
        a = randomMatrix(ROWS, COLS);
        b = randomMatrix(ROWS, COLS);
        c = newMatrix(ROWS, COLS);
    }

    int chunk_rows = ROWS / size;
    float** a_chunk = newMatrix(chunk_rows, COLS);
    float** b_chunk = newMatrix(chunk_rows, COLS);
    float** c_chunk = newMatrix(chunk_rows, COLS);

    MPI_Scatter(*a, chunk_rows * COLS, MPI_FLOAT, *a_chunk, chunk_rows * COLS, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(*b, chunk_rows * COLS, MPI_FLOAT, *b_chunk, chunk_rows * COLS, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk_rows; i++) {
        for (int j = 0; j < COLS; j++) {
            c_chunk[i][j] = a_chunk[i][j] * b_chunk[i][j];
        }
    }
    MPI_Gather(*c_chunk, chunk_rows * COLS, MPI_FLOAT, *c, chunk_rows * COLS, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << toString(c, ROWS, COLS) << endl;
    }
    MPI_Finalize();
}
