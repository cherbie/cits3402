#include "sp.h"
/**
 * Read the value of the input file.
 * @return 0 to indicate success and -1 to indicate failure.
 */
int read_input_mpi(SP_CONFIG *config, PATHS *paths) {
    int err = MPI_File_open(MPI_COMM_WORLD, (*config).filename_in, MPI_MODE_RDONLY, MPI_INFO_NULL, &(*config).file_in); // NO INFO PROVIDED
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }

    MPI_Status status;
    MPI_Offset filesize = 0;
    MPI_File_get_size((*config).file_in, &filesize);
    int nnodes;

    // -- GET THE NUMBER OF NODES --
    MPI_File_set_view((*config).file_in, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    MPI_File_read_all((*config).file_in, &nnodes, 1, MPI_INT, &status);
    printf(" number of nodes: %i\n", nnodes);

    // -- GET THE ADJCENCY MATRIX --
    MPI_Datatype mpi_vectors; // ~ array of path weights for a particular node
    MPI_Datatype mpi_row;
    int nrows = nnodes/(*config).nproc; // number of rows handled by the system
    int stride = (*config).nproc;
    int nints = nnodes*nnodes/(*config).nproc;

    MPI_Type_contiguous(nnodes, MPI_INT, &mpi_row);
    MPI_Type_commit(&mpi_row);
    MPI_Type_vector(nrows, 1, stride, mpi_row, &mpi_vectors);
    MPI_Type_commit(&mpi_vectors);

    int *weights = calloc(nints, sizeof(int));
    if(weights == NULL) return -1;

    MPI_File_set_view((*config).file_in, 4 + nnodes*sizeof(int)*(*config).rank, mpi_row, mpi_vectors, "native", MPI_INFO_NULL);
    MPI_File_read_all((*config).file_in, weights, nints, MPI_INT, &status);

    if((*config).rank == 2) {
        int j = 0;
        for(int i = 0; i < nints; i++) {
            j = i % nnodes;
            printf("%i ", weights[i]);
            if(j == (nnodes -1)) printf("\t|\t%i\n", i/nnodes);
        }
        printf("-------\n");
    }
    else return 0;

    /* Need master slave relationship to gather all data in the master matrix
       - Blocking function may be required.
       - after that broadcast the data to all nodes in MPI_COMM_WORLD.
       - Can then start computation.
     */
    //receiveAllChildData(); // function that collects all data in master & broadbast this to the slave after. start of adjanceny matrix.

    free(weights);
    if((*config).rank == ROOT) printf("Processing input file.\n");
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
