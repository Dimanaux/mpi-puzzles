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
    float* x;
    float* a;
    float* result = new float[COLS];

    int chunk_rows = ROWS / size;
    float* a_chunk = new float[chunk_rows * COLS];

    float* result_chunk = new float[chunk_rows];

    x = randomFloatArray(COLS);
    if (rank == 0) {
        a = randomFloatArray(ROWS * COLS);
    }

    MPI_Bcast(x, COLS, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, chunk_rows * COLS, MPI_FLOAT, a_chunk, chunk_rows * COLS, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk_rows; i++) {
        float sum = 0;
        for (int j = 0; j < COLS; j++) {
            sum += a_chunk[i * COLS + j] * x[j];
        }
        result_chunk[i] = sum;
    }
    MPI_Gather(result_chunk, chunk_rows, MPI_FLOAT, result, COLS, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << toString(x, COLS) << endl;
    }
    MPI_Finalize();
}
