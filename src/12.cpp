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

    int* vector1;
    int* vector2;
    int scalar_product;
	
    if (rank == 0) {
		vector1 = randomArray(VECTOR_SIZE);
		vector2 = randomArray(VECTOR_SIZE);
	}

	int chunk_size = VECTOR_SIZE / size;
	int* chunk1 = new int[chunk_size];
	int* chunk2 = new int[chunk_size];

	MPI_Scatter(vector1, chunk_size, MPI_INT, chunk1, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(vector2, chunk_size, MPI_INT, chunk2, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    
	int partial_product = 0;
	for (int i = 0; i < chunk_size; i++) {
		partial_product += chunk1[i] * chunk2[i];
	}

	MPI_Reduce(&partial_product, &scalar_product, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
    if (rank == 0) {
		cout << scalar_product << endl;
    }
	MPI_Finalize();
}
