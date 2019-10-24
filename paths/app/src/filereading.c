#include "sp.h"
/**
 * Read the value of the input file.
 * @return 0 to indicate success and -1 to indicate failure.
 */
int read_file_mpi(SP_CONFIG *config, PATHS *paths) {
    // -- OPEN FILE -
    int err = MPI_File_open(MPI_COMM_WORLD, (*config).filename_in, MPI_MODE_RDONLY, MPI_INFO_NULL, &(*config).file_in); // NO INFO PROVIDED
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }

    // -- GENERIC VARIABLES --
    MPI_Status status;

    // -- GET THE NUMBER OF NODES --
    err = MPI_File_set_view((*config).file_in, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    err = MPI_File_read_all((*config).file_in, &(*paths).nodes, 1, MPI_INT, &status);
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }
    printf(" number of nodes: %i\n", (*paths).nodes);

    // -- GET THE ADJCENCY MATRIX --
    MPI_Datatype mpi_block;
    int offset, nelem;
    int remainder = (*paths).nodes % (*config).nproc; // remainder of even block distribution
    int rowsperblk = ((*paths).nodes - remainder) / (*config).nproc; // number of rows handled by each node

    if((*config).rank == ROOT) {
        offset = (rowsperblk * (*paths).nodes) * ((*config).nproc - 1);
        nelem = (rowsperblk + remainder) * (*paths).nodes;
    }
    else {
        offset = (rowsperblk * (*paths).nodes) * ((*config).rank - 1);
        nelem = rowsperblk * (*paths).nodes;
    }

    MPI_Type_contiguous(nelem, MPI_INT, &mpi_block);
    MPI_Type_commit(&mpi_block);

    int *weights = calloc(nelem, sizeof(int));
    if(weights == NULL) return -1;

    MPI_File_set_view((*config).file_in, 4 + (offset * sizeof(int)), MPI_INT, mpi_block, "native", MPI_INFO_NULL);
    MPI_File_read_all((*config).file_in, weights, nelem, MPI_INT, &status);


    // -- COLLATE DISTRIBUTED FILE READING --
    (*paths).weight = malloc(((*paths).nodes * (*paths).nodes) * sizeof(int));
    if((*paths).weight == NULL) {
        perror(NULL);
        return -1;
    }

    int *recvcounts = malloc((*config).nproc * sizeof(int *));
    int *displs = malloc((*config).nproc * sizeof(int *));
    if(recvcounts == NULL || displs == NULL) {
        perror(NULL);
        return -1;
    }
    if(get_merge_info(config, paths, &offset, &nelem, &recvcounts, &displs)) { // GET INFO TO CONDUCT MERGE
        fprintf(stderr, "Error merging distributed data\n");
        return -1;
    }

    err = MPI_Allgatherv(&weights[0], nelem, MPI_INT, &(*paths).weight[0], recvcounts, displs, MPI_INT, MPI_COMM_WORLD);
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }

    MPI_Barrier(MPI_COMM_WORLD); // WAIT FOR ALL COMMUNICATION

    /*if((*config).rank == ROOT){
        for(int i = 0; i < ((*paths).nodes *(*paths).nodes); i++) {
            if((i % (*paths).nodes) == 0) printf("\n");
            printf("%i ", (*paths).weight[i]);
        }
    }*/

    // -- DEALLOC TEMPORARY VARIABLES --
    MPI_Type_free(&mpi_block);
    free(weights);
    free(recvcounts);
    free(displs);

    //if((*config).rank == ROOT) printf("Processing input file.\n");
    return 0;
}


/*int process_matrix_array(SP_CONFIG *config, PATHS *paths, int *buf) {
    int size = (*paths).nodes;
    int work = size/(*config).nproc; // number of rows collected by each node
    (*paths).weight = malloc(size * sizeof(int *));
    if((*paths).weight == NULL) {
    	perror(NULL);
    	return -1;
    }
    int j = 0, node = 0, element = 0;
    printf("num processes = %i | size = %i\n", (*config).nproc, size);
    for(int i = 0; i < size; i++) {
        node = i/work; // the node that read the file
        element = i%work*(*config).nproc; // the order of row read by file.
        j = node + element;
        (*paths).weight = calloc(size, sizeof(int));
        if((*paths).weight == NULL) {
            perror(NULL);
            return -1;
        }
        memcpy(&(*paths).weight[j][0], &buf[i * size], size * sizeof(int));
    }
    return 0;
}*/


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
