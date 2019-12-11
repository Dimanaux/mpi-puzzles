#include <iostream>

#include "mpi.h"

#include "lib.h"

using namespace std;

const int VECTOR_SIZE = 144;

int main(int argc, char** argv) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float* vector;
    FloatInt min;

    if (rank == 0) {
		vector = randomFloatArray(VECTOR_SIZE);
        logGot(rank, toString(vector, VECTOR_SIZE));
	}

	int chunk_size = VECTOR_SIZE / size;
	float* chunk = new float[chunk_size];

	MPI_Scatter(vector, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    
	FloatInt local_min = {chunk[0], 0};
	for (int i = 0; i < chunk_size; i++) {
		if (chunk[i] < local_min.value) {
            local_min.index = i;
        }
	}
    local_min.value = chunk[local_min.index];

	MPI_Reduce(&local_min.index, &min.index, 1, MPI_FLOAT_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        min.value = vector[min.index];
		cout << "min " + to_string(min.value) + " at index " + to_string(min.index) << endl;
    }
	MPI_Finalize();
}
