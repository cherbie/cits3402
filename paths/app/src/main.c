#include "sp.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); // critical
    SP_CONFIG *config = set_config();
    if(config != NULL) {
        if(parse_args(&(config[0]), &argc, &argv)) {
            fprintf(stderr, "Unable to parse command line arguments.\n");
            return EXIT_FAILURE;
        }
        if(read_input(&(config[0]))) {
            fprintf(stderr, "Unable to read input file.\n");
            return EXIT_FAILURE;
        }
        if(config[0].rank == ROOT) printf("Hello World\n");
        dealloc_config(config);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

/**
 * Set config struct for distributed nodes.
 */
SP_CONFIG *set_config(void) {
    SP_CONFIG *config = malloc(1 * sizeof(SP_CONFIG));
    if(config == NULL) {
        perror(NULL);
        return NULL;
    }
    MPI_Comm_size(MPI_COMM_WORLD, &(*config).size);
    MPI_Comm_rank(MPI_COMM_WORLD, &(*config).rank);
    printf("Information:\nSize = %i\nRank = %i\n", (*config).size, (*config).rank);
    return config;
}
