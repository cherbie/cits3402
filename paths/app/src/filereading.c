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
    //printf(" number of nodes: %i\n", nnodes);
    (*paths).nodes = nnodes;

    // -- GET THE ADJCENCY MATRIX --
    MPI_Datatype mpi_vectors; // ~ array of path weights for a particular node
    MPI_Datatype mpi_row;
    int nrows = nnodes/(*config).nproc; // number of rows handled by the system
    int stride = (*config).nproc;
    int nints = nnodes*nnodes/(*config).nproc; //number of integers per node

    MPI_Type_contiguous(nnodes, MPI_INT, &mpi_row);
    MPI_Type_commit(&mpi_row);
    MPI_Type_vector(nrows, 1, stride, mpi_row, &mpi_vectors);
    MPI_Type_commit(&mpi_vectors);

    int *weights = calloc(nints, sizeof(int));
    if(weights == NULL) return -1;

    MPI_File_set_view((*config).file_in, 4 + nnodes*sizeof(int)*(*config).rank, mpi_row, mpi_vectors, "native", MPI_INFO_NULL);
    MPI_File_read_all((*config).file_in, weights, nints, MPI_INT, &status);

    MPI_Datatype mpi_int_array; // contiguous MPI_INT
    MPI_Type_contiguous(nints, MPI_INT, &mpi_int_array);
    MPI_Type_commit(&mpi_int_array);

    int *recvbuf = malloc(nnodes * nnodes * sizeof(int));
    err = MPI_Allgather(&weights[0], 1, mpi_int_array, &recvbuf[0], 1, mpi_int_array, MPI_COMM_WORLD);
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }

    MPI_Barrier(MPI_COMM_WORLD); // wait for all communication

    if(process_matrix_array(&config[0], &paths[0], &recvbuf[0])) {
        fprintf(stderr, "Error sharing file data with communication group.\n");
        return -1;
    }

    MPI_Type_free(&mpi_row);
    MPI_Type_free(&mpi_vectors);
    MPI_Type_free(&mpi_int_array);
    free(weights);
    free(recvbuf);

    return 0;
}


int process_matrix_array(SP_CONFIG *config, PATHS *paths, int *buf) {
    int size = (*paths).nodes;
    int work = size/(*config).nproc; // number of rows collected by each node
    (*paths).weight = malloc(size * sizeof(int *));
    if((*paths).weight == NULL) {
    	perror(NULL);
    	return -1;
    }
    int j = 0, node = 0, element = 0;
    //printf("num processes = %i | size = %i\n", (*config).nproc, size);
    for(int i = 0; i < size; i++) {
        node = i/work; // the node that read the file
        element = i%work*(*config).nproc; // the order of row read by file.
        j = node + element;
        (*paths).weight[j] = calloc(size, sizeof(int));
        if((*paths).weight[j] == NULL) {
            perror(NULL);
            return -1;
        }
        memcpy(&(*paths).weight[j][0], &buf[i * size], size * sizeof(int));
    }
    return 0;
}
