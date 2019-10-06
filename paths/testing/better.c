#include "mpi.h"
#include <stdio.h>

void error_handle(int);

int main(int argc, char *argv[]) {
    int rank, size, status;
    MPI_Init(&argc, &argv);
    status = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    error_handle(status);
    status = MPI_Comm_size(MPI_COMM_WORLD, &size);
    error_handle(status);
    printf("I am process %d of %d\n", rank, size);
    MPI_Finalize();

    return 0;
}

void error_handle(int status) {
    switch(status) {
        case MPI_SUCCESS:
            break;
        case 1:
            printf("help\n");
            break;
        default:
            break;
    }
}
