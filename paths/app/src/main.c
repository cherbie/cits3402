#include "sp.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); // critical
    SP_CONFIG *config = malloc(1 * sizeof(SP_CONFIG));
    PATHS *paths = malloc(1 * sizeof(PATHS));
    if(paths == NULL || config == NULL) {
        perror(NULL);
        return -1;
    }
    if(initialise(config, paths)) return EXIT_FAILURE;
    // -- PARSE COMMAND LINE ARGUMENTS --
    if(parse_args(&(config[0]), &argc, &argv)) {
        fprintf(stderr, "Unable to parse command line arguments.\n");
        return EXIT_FAILURE;
    }
    // -- READ INPUT FILE --
    if(read_file_mpi(&(config[0]), &(paths[0]))) {
        fprintf(stderr, "Unable to read input file.\n");
        return EXIT_FAILURE;
    }
    // -- ALGORITHM --
    /*
    if(block_apsp(&config[0], &paths[0])) {
        fprintf(stderr, "Unable to calculate the all pairs shortest paths\n");
        return -1;
    }*/
    printf("reached\n");
    if(compute_shortest_paths(&config[0], &paths[0])) {
        fprintf(stderr, "Unable to calculate the all pairs shortest paths\n");
        return EXIT_FAILURE;
    }
    if((*config).rank == ROOT) print_matrix((*paths).sp, &(*paths).nodes);

    printf(" -- Completed calculation. -- \n");

    // -- DEALLOCATE MEMORY --
    //if((*config).rank == ROOT) free_mtx((void **)(*paths).sp, &(*paths).nodes);
    dealloc_config(config);
    dealloc_paths(paths);

    MPI_Finalize();
    return EXIT_SUCCESS;
}

/**
 * Set config struct for distributed nodes.
 */
int initialise(SP_CONFIG *config, PATHS *paths) {
    MPI_Comm_size(MPI_COMM_WORLD, &(*config).nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &(*config).rank);
    printf("Information:\nSize = %i\nRank = %i\n", (*config).nproc, (*config).rank);

    return 0;
}
