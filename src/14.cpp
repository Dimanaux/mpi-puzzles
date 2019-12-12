#include <iostream>

#include "mpi.h"

#include "lib.h"

using namespace std;

const int ROWS = 12;
const int COLS = 12;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float** matrix;
    FloatInt max = {0, 0};

    if (rank == 0) {
        matrix = randomMatrix(ROWS, COLS);
    }

    int chunk_rows = ROWS / size;
    float** chunk = newMatrix(chunk_rows, COLS);

    MPI_Scatter(*matrix, chunk_rows * COLS, MPI_FLOAT, *chunk, chunk_rows * COLS, MPI_FLOAT, 0, MPI_COMM_WORLD);

    FloatInt max_sum = {0, 0};
    for (int i = 0; i < chunk_rows; i++) {
        float sum = 0;
        for (int j = 0; j < COLS; j++) {
            sum += abs(chunk[i][j]);
        }
        if (sum > max_sum.value) {
            max_sum.value = max_sum.index = sum;
        }
    }

    MPI_Reduce(&max_sum, &max, 1, MPI_FLOAT_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << max.value << " at " << max.index << endl;
    }
    MPI_Finalize();
}
