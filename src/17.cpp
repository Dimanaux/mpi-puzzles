#include <iostream>
#include <string>

#include "mpi.h"

#include "lib.h"

using namespace std;

void Gather(float* send_data,
            int send_count,
            float* recv_data,
            int recv_count,
            int root) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    if (rank == root) {
        for (int i = 0; i < send_count; i++) {
            recv_data[recv_count * rank + i] = send_data[i];
        }
        for (int i = 0; i < size; i++) {
            if (i == root) continue;
            MPI_Recv(
                recv_data + (i > root ? i : (i - 1)) * recv_count,
                recv_count, MPI_FLOAT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status
            );
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (i == root) continue;
            MPI_Send(send_data, send_count, MPI_FLOAT, root, 99, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int v_size = 24;
    float* v;
    if (rank == 0) {
        v = new float[v_size];
    }
    const int chunk_size = v_size / size;
    float* chunk = new float[chunk_size];
    for (int i = 0; i < chunk_size; i++) {
        chunk[i] = rank * i;
    }

    Gather(chunk, chunk_size, v, chunk_size, 0);
    // MPI_Gather(chunk, chunk_size, MPI_FLOAT, v, chunk_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << toString(v, v_size) << endl;
    }
    MPI_Finalize();
}
