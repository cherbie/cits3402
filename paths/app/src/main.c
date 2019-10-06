#include "sp.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); // critical
    printf("Hello world\n");
    MPI_Finalize();
    return EXIT_SUCCESS;
}
