#include "sp.h"

/**
 * Read the value of the input file.
 * @return 0 to indicate success and -1 to indicate failure.

int read_input_mpi(SP_CONFIG *config) {
    int err = MPI_File_open(MPI_COMM_WORLD, (*config).filename_in, MPI_MODE_RDONLY, MPI_INFO_NULL, &(*config).file_in); // NO INFO PROVIDED
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }
    if((*config).rank == ROOT) printf("Processing input file.\n");
    PATHS *edges = malloc(1 * sizeof(PATHS));
    if(edges == NULL) {
        perror(NULL);
        return -1;
    }
    return 0;
}
*/

int read_input(SP_CONFIG *config, PATHS *paths) {
    (*config).fp_in = fopen((*config).filename_in, "rb");
    if((*config).fp_in == NULL) {
        perror(NULL);
        return -1;
    }
    //find the number of nodes
    size_t rtn = fread(&(*paths).nodes, sizeof(uint16_t), 1, (*config).fp_in);
    if(rtn == 0) {
        perror(NULL);
        return -1;
    }
    fprintf(stdout, "%zu | %hu\n", rtn, (*paths).nodes);
    return 0;
}

/**
 * Create the output filename.
 * @return 0 to indicate success and -1 to indicate failure.
 */
int create_output(SP_CONFIG *config, char *name) {
    /*(*config).filename_out = strdup(name);
    if((*config).filename_out == NULL) {
        perror(NULL);
        return -1;
    }
    (*config).fd_out = fopen((*config).filename_out, "wb");
    if((*config).fd_out == NULL) {
        perror(NULL);
        return -1;
    }
    printf("Creating output file.\n");*/
    return 0;
}
