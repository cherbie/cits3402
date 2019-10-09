#include "sp.h"
#define INTS_PER_BLK 1
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
    /*int bufsize = (filesize-2)/(*config).nproc;
    //int *buf = malloc(bufsize);
    int nints = bufsize/sizeof(int);
    int gsizes[2], distribs[2], dargs[2], psizes[2];
    gsizes[0] = nnodes;
    distribs[0] = MPI_DISTRIBUTE_CYCLIC;
    dargs[0] = 1;
    psizes[0] = 2;
    gsizes[1] = nnodes;
    distribs[1] = MPI_DISTRIBUTE_NONE;
    dargs[1] = 0;
    psizes[1] = 1;
    int **weights = malloc(nnodes * sizeof(int*));
    if(weights == NULL) return -1;
    for(int i = 0; i < nnodes; i++) {
        weights[i] = malloc(nnodes * sizeof(int));
        if(weights[i] == NULL) return -1;
    }


    err = MPI_Type_create_darray((*config).nproc, (*config).rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C, MPI_INT, &path_weights);
    */
    MPI_Datatype path_weights;
    //int bufsize = (filesize-2)/(*config).nproc;
    int **weights = malloc(nnodes * sizeof(int*));
    if(weights == NULL) return -1;
    for(int i = 0; i < nnodes; i++) {
        weights[i] = malloc(nnodes * sizeof(int));
        if(weights[i] == NULL) return -1;
    }
    //int ints_per_blk = nnodes;
    //int nints = bufsize/sizeof(int);
    MPI_Type_contiguous(nnodes, MPI_INT, &path_weights);
    if(err != MPI_SUCCESS) {
        error_handler(&err);
        return -1;
    }
    MPI_Type_commit(&path_weights);
    for(int i = (*config).rank; i < nnodes; i += (*config).nproc) {
        MPI_File_set_view((*config).file_in, (4+(i*nnodes*4)), MPI_INT, path_weights, "native", MPI_INFO_NULL); // set view to the location required
        MPI_File_read_all((*config).file_in, &weights[i][0], 1, path_weights, &status);
    }
    //MPI_File_read_shared((*config).file_in, &weights[0][0], nnodes*nnodes, MPI_INT, &status);



/*
    MPI_Datatype weights;
    MPI_Type_vector(nints/INTS_PER_BLK, INTS_PER_BLK, INTS_PER_BLK*(*config).nproc, MPI_INT, &weights);
    MPI_Type_commit(&weights);
    MPI_File_set_view((*config).file_in, INTS_PER_BLK*sizeof(int)*(*config).rank, MPI_INT, weights, "native", MPI_INFO_NULL);
    MPI_File_read_all((*config).file_in, buf, nints, MPI_INT, &status);
*/
    if((*config).rank == ROOT) {
        //int count = 0;
        for(int i = 0; i < nnodes; i++) {
            int j = i % nnodes;
            for(j = 0; j < nnodes; j++) printf("%i ", weights[i][j]);
            printf("\n");
        }
        printf("-------");
    }
    else if((*config).rank == 1) {
        for(int i = 0; i < nnodes; i++) {
            int j = i % nnodes;
            for(j = 0; j < nnodes; j++) printf("%i ", weights[i][j]);
            printf("\n");
        }
    }
    else return 0;
    //free(buf);
    if((*config).rank == ROOT) printf("Processing input file.\n");
    return 0;
}

/*
int read_input(SP_CONFIG *config, PATHS *paths) {
    (*config).fp_in = fopen((*config).filename_in, "rb");
    if((*config).fp_in == NULL) {
        perror(NULL);
        return -1;
    }
    //find the number of nodes
    size_t rtn = fread(&(*paths).nodes, sizeof(uint16_t), 1, (*config).fp_in);
    if(rtn != 1) {
        perror(NULL);
        return -1;
    }
    fprintf(stdout, "%zu | %hu\n", rtn, (*paths).nodes);
    (*paths).weight = malloc((*paths).nodes * sizeof(uint16_t));
    if((*paths).weight == NULL) {
        perror(NULL);
        return -1;
    }
    int count = 0;
    int node = 0;
    int index = 0;
    (*paths).weight[node] = malloc((*paths).nodes * sizeof(uint16_t));
    if((*paths).weight[node] == NULL) {
        perror(NULL);
        return -1;
    }
    //unsigned char element[2];
    //bool spacer = false;
    while(fread(&(*paths).weight[node][index], sizeof(unsigned char), 2, (*config).fp_in) == 2) {
        node = count/(*paths).nodes;
        if(node >= (*config).size) break;
        index = count%(*paths).nodes;
        //if(element[2] == '\0' && element[3] == '\0') continue;
        if(index == ((*paths).nodes-1) && node < (*paths).nodes) {
            (*paths).weight[node+1] = malloc((*paths).nodes * sizeof(uint16_t));
            if((*paths).weight[node] == NULL) {
                perror(NULL);
                return -1;
            }
        }
        //memcpy(&(*paths).weight[node][index], &element[0], sizeof(uint16_t));
        fprintf(stdout, "--> %hu\n", (*paths).weight[node][index]);
        if(fseek((*config).fp_in, 2, SEEK_CUR)) fprintf(stderr, "Error with fseek()");
        count++;
    }
    fprintf(stdout, "COMPLETED FILE READING\n");
    return 0;
}
*/
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
