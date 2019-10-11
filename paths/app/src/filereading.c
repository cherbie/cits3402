#include "sp.h"
/**
 * Read the value of the input file.
 * @return 0 to indicate success and -1 to indicate failure.
 */
int read_file_mpi(SP_CONFIG *config, PATHS *paths) {
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
    err = MPI_File_set_view((*config).file_in, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    err = MPI_File_read_all((*config).file_in, &nnodes, 1, MPI_INT, &status);
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }
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

    if((*config).rank == 0) {
        int j = 0;
        for(int i = 0; i < nints; i++) {
            j = i % nnodes;
            printf("%i ", weights[i]);
            if(j == (nnodes -1)) printf("\t|\t%i\n", i/nnodes);
        }
        printf("-------\n");
    }

    // -- GATHER DATA --
    /* Need master slave relationship to gather all data in the master matrix
       - Blocking function may be required.
       - after that broadcast the data to all nodes in MPI_COMM_WORLD.
       - Can then start computation.
     */

    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Datatype mpi_int_array; // contiguous MPI_INT
    MPI_Type_contiguous(nints, MPI_INT, &mpi_int_array);
    MPI_Type_commit(&mpi_int_array);

    int *recvbuf = malloc((*config).nproc * nints * sizeof(int)); //nnodes^2 integers
    err = MPI_Allgather(&weights[0], 1, mpi_int_array, &recvbuf[0], 1, mpi_int_array, MPI_COMM_WORLD);
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }
    if((*config).rank == 2) {
        for(int i = 0; i < (nnodes*nnodes); i++) {
            printf("%i ", recvbuf[i]);
            if((i%nnodes) == nnodes-1) printf("\n");
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); // wait for all communication

    process_matrix_array(&paths[0], recvbuf);

    MPI_Type_free(&mpi_row);
    MPI_Type_free(&mpi_vectors);
    MPI_Type_free(&mpi_int_array);
    free(weights);
    free(recvbuf);

    if((*config).rank == ROOT) printf("Processing input file.\n");
    return 0;
}


int process_matrix_array(SP_CONFIG *config, PATHS *paths, int *buf) {
    int size = (*paths).nodes;
    (*paths).weights = malloc(size * sizeof(int *));
    if((*paths).weights == NULL) {
	perror(NULL);
	return -1;
    } 
    int j = 0;
    for(int i = 0; i < size; i++) {
	(*paths).weights[i] = calloc(size, sizeof(int));
	if((*paths).weights[i] == NULL) {
 	    perror(NULL);
	    return -1;
	}
        int node = i/(*config).nproc;
	int element = i%(*config).nproc*(*config).nproc;
        j = node + element;
	if((*config).rank == ROOT) printf("j is => %i\n", j);
    }
    return 0;
}


/**
 *
 * @param config the execution details
 * @param size the number of elements in the weights buffer.
 * @param weights the path weights read from file. (sendbuf)
 */
int receiveAllData(SP_CONFIG *config, int *weights, int size) {
    return 1;


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
