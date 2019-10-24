#include "sp.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); // CRITICAL

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
    if(compute_shortest_paths(&config[0], &paths[0])) {
        fprintf(stderr, "Unable to calculate the all pairs shortest paths\n");
        return EXIT_FAILURE;
    }
    if((*config).rank == ROOT) {
        print_matrix((*paths).sp, &(*paths).nodes);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    printf(" -- Completed calculation. -- \n");

    // -- DEALLOCATE MEMORY --
    if((*config).rank == ROOT) {
        free((*paths).sp);
    }
    dealloc_config(config);
    free(paths);
    free(config);

    // -- FINALIZE DISTRIBUTED NODES --
    MPI_Finalize();
    return EXIT_SUCCESS;
}
