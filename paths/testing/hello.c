#include "mpi.h" // need this for the mpi function declerations
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); // critical
    printf("Hello world\n"); // critical
    MPI_Finalize();
    return 0;
}
