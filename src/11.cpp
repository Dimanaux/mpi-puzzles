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

    int* vector;
    int sum;
	
    if (rank == 0) {
		vector = randomArray(VECTOR_SIZE);
	}

	int chunk_size = VECTOR_SIZE / size;
	int* chunk = new int[chunk_size];

	MPI_Scatter(vector, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    
	int partial_norm = 0;
	for (int i = 0; i < chunk_size; i++) {
		partial_norm += abs(chunk[i]);
	}

	MPI_Reduce(&partial_norm, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
    if (rank == 0) {
		cout << sum << endl;
    }
	MPI_Finalize();
}
