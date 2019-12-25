#include <iostream>
#include <string>

#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double t1, t2;

    float x = size - rank + 10;
    float sum = 0;

    t1 = MPI_Wtime();
    MPI_Reduce(&x, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&sum, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    t2 = MPI_Wtime();

    if (rank == 0) {
        cout << "Reduce + Bcast takes " << t2 - t1 << "sec\n";
    }

    t1 = MPI_Wtime();
    MPI_Allreduce(&x, &sum, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    t2 = MPI_Wtime();

    if (rank == 0) {
        cout << "AllReduce takes " << t2 - t1 << "sec\n";
    }
    MPI_Finalize();
}
