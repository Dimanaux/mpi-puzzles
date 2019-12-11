#include <iostream>

#include "mpi.h"

#include "lib.h"

using namespace std;

const int ARRAY_SIZE = 3500;
const int REPEATS = 5;

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    int* array = randomArray(ARRAY_SIZE);

    if (rank == 0) {
        // a.
        for (int i = 0; i < REPEATS; i++) {
            double t1 = MPI_Wtime();
            MPI_Send(array, ARRAY_SIZE, MPI_INT, 1, 99, MPI_COMM_WORLD);
            MPI_Recv(array, ARRAY_SIZE, MPI_INT, 1, 99, MPI_COMM_WORLD, &status);
            double t2 = MPI_Wtime();
            cout << "sending and receiving took " + to_string(t2 - t1) + " sec" << endl;
        }
        // b.
        for (int i = 0; i < REPEATS; i++) {
            double t1 = MPI_Wtime();
            MPI_Send(array, ARRAY_SIZE, MPI_INT, 1, 99, MPI_COMM_WORLD);
            double t2 = MPI_Wtime();
            cout << "just sending took " + to_string(t2 - t1) + " sec" << endl;
        }
    } else if (rank == 1) {
        // a.
        for (int i = 0; i < REPEATS; i++) {
            MPI_Recv(array, ARRAY_SIZE, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
            MPI_Send(array, ARRAY_SIZE, MPI_INT, 0, 99, MPI_COMM_WORLD);
        }
        // b.
        for (int i = 0; i < REPEATS; i++) {
            MPI_Recv(array, ARRAY_SIZE, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        }
    }

    MPI_Finalize();
}
