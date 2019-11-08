#include <stdio.h>

#include "mpi.h"

/**
 * Building:
 * ./build 01
 * OR
 * mpic++ 01.cpp -o 01
 *
 * Running:
 * ./run -n 4 01
 * OR
 * mpirun -np 4 ./01
 */
int main(int argc, char **argv)
{
    int nPr, size;
    MPI_Init(&argc, &argv);
    printf("Hello, world!\n");
    MPI_Finalize();
}
