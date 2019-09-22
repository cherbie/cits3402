#include "mop.h"

/**
 * Start of matrix operations.
 * Return 1 to indicate success and 0 to indicate failure.
 */
int operation_main(void) {
    if(!(*op_func[config.operation])()) {
        fprintf(stderr, "Unable to perform matrix operation.\n");
        return 0;
    }
    print(" ... end of operation main function.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int scalar(void) {
    //SPARSE MATRIX REP.
    const int num_files = 1; int file_id = 0;
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_files * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    gettimeofday(&config.time[0].start, NULL);
    if(!read_to_coo(&coo_sparse_mtx, 0, file_id)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    gettimeofday(&config.time[0].end, NULL);
    gettimeofday(&config.time[1].start, NULL);
    if(!process_scalar(&coo_sparse_mtx[0], config.sm)) {
        fprintf(stderr, "Error performing scalar multiplication.\n");
        return 0;
    }
    gettimeofday(&config.time[1].end, NULL);
    print_coo(&coo_sparse_mtx[0]);

    if(!process_stat()) fprintf(stderr, "Error determining duration of operations.\n");

    // -- LOG RESULT TO FILE --
    if(config.log_fd != NULL) { //specified
        fprintf(config.log_fd, "%s\n", config.op_str);
        for(int i = 0; i < num_files; i++) {
            fprintf(config.log_fd, "%s\n", config.filename[i]);
        }
        fprintf(config.log_fd, "%i\n", config.num_threads);
        if(!log_coo_result(&coo_sparse_mtx[0], config.log_fd)) {
            fprintf(stderr, "Error logging result to file.\n");
            return 0;
        }
        fprintf(config.log_fd, "%5.3f\n%5.3f\n", config.time[0].delta, config.time[1].delta);
    }

    dealloc_coo(&coo_sparse_mtx, num_files);
    free(coo_sparse_mtx);
    print(" ... completed scalar multiplication & deallocated memory");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int trace(void) {
    const int num_files = 1;
    //SPARSE MATRIX REP.
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_files * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    gettimeofday(&config.time[0].start, NULL);
    if(!read_to_coo(&coo_sparse_mtx, 0, 0)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    gettimeofday(&config.time[0].end, NULL);
    int i; float f;
    gettimeofday(&config.time[1].start, NULL);
    if(!process_trace(&coo_sparse_mtx[0], &i, &f)) {
        fprintf(stderr, "Error calulating the trace of the matrix provided.\n");
        return 0;
    }
    gettimeofday(&config.time[1].end, NULL);
    if(!process_stat()) fprintf(stderr, "Error determining duration of operations.\n");

    // -- LOG RESULT TO FILE --
    if(config.log_fd != NULL) { //specified
        fprintf(config.log_fd, "%s\n", config.op_str);
        for(int i = 0; i < num_files; i++) {
            fprintf(config.log_fd, "%s\n", config.filename[i]);
        }
        fprintf(config.log_fd, "%i\n", config.num_threads);
        if(!log_trace_result(&coo_sparse_mtx[0], &i, &f, config.log_fd)) {
            fprintf(stderr, "Error logging the result of the trace.\n");
            return 0;
        }
        fprintf(config.log_fd, "%5.3f\n%5.3f\n", config.time[0].delta, config.time[1].delta);
    }

    dealloc_coo(&coo_sparse_mtx, num_files);
    free(coo_sparse_mtx);
    print(" ... finding trace of matrix.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int addition(void) {
    const int num_coo = 3;
    //SPARSE MATRIX REP.
    int coo_id = 0;
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_coo * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    gettimeofday(&config.time[0].start, NULL);
    for(int k = 0; k < NUMBER_OF_INPUT_FILES; k++) {
        if(!read_to_coo(&coo_sparse_mtx, coo_id++, k)) {
            fprintf(stderr, "Error converting file to sparse matrix form.\n");
            return 0;
        }
    }
    gettimeofday(&config.time[0].end, NULL);
    gettimeofday(&config.time[1].start, NULL);
    if(!process_addition(&coo_sparse_mtx)) {
        fprintf(stderr, "Error performing matrix addition.\n");
        return 0;
    }
    gettimeofday(&config.time[1].end, NULL);
    if(!process_stat()) fprintf(stderr, "Error determining duration of operations.\n");

    print_coo(&(coo_sparse_mtx[2]));

    // -- LOG RESULT TO FILE --
    if(config.log_fd != NULL) { //specified
        fprintf(config.log_fd, "%s\n", config.op_str);
        for(int i = 0; i < NUMBER_OF_INPUT_FILES; i++) {
            fprintf(config.log_fd, "%s\n", config.filename[i]);
        }
        fprintf(config.log_fd, "%i\n", config.num_threads);
        if(!log_coo_result(&coo_sparse_mtx[2], config.log_fd)) {
            fprintf(stderr, "Error logging result to file.\n");
            return 0;
        }
        fprintf(config.log_fd, "%5.3f\n%5.3f\n", config.time[0].delta, config.time[1].delta);
    }


    dealloc_coo(&coo_sparse_mtx, num_coo);
    free(coo_sparse_mtx);
    print(" ... performing matrix addition.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int transpose_matrix(void) {
    const int num_files = 2;
    //SPARSE MATRIX REP CSR.
    CSR *csr_sparse_mtx; CSC *csc_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((csr_sparse_mtx = malloc(1 * sizeof(CSR))) == NULL) {
        perror(NULL);
        return 0;
    }
    if((csc_sparse_mtx = malloc(1 * sizeof(CSC))) == NULL) {
        perror(NULL);
        return 0;
    }
    gettimeofday(&config.time[0].start, NULL);
    if(!read_to_csr(&csr_sparse_mtx, 0, 0)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    gettimeofday(&config.time[0].end, NULL);
    print_csr(&csr_sparse_mtx[0]); //debugging
    print("FINISHED READING CSR FILE\n");
    gettimeofday(&config.time[1].start, NULL);
    if(!process_transpose(&csr_sparse_mtx[0], &csc_sparse_mtx[0])) {
        fprintf(stderr, "Error transposing given matrix.\n");
        return 0;
    }
    gettimeofday(&config.time[1].end, NULL);
    if(!process_stat()) fprintf(stderr, "Error determining duration of operations.\n");

    print_csc(&csc_sparse_mtx[0]);

    printf("memcompare = %i\n", memcmp(&csr_sparse_mtx[0].mtxi, &csc_sparse_mtx[0].mtxi, csc_sparse_mtx[0].size * sizeof(int)));

    // -- LOG RESULT TO FILE --
    if(config.log_fd != NULL) { //specified
        fprintf(config.log_fd, "%s\n", config.op_str);
        for(int i = 0; i < num_files; i++) {
            fprintf(config.log_fd, "%s\n", config.filename[i]);
        }
        fprintf(config.log_fd, "%i\n", config.num_threads);
        if(!log_csc_ts_result(&csc_sparse_mtx[0], config.log_fd)) {
            fprintf(stderr, "Error logging transposed matrix to file.\n");
            return 0;
        }
        fprintf(config.log_fd, "%5.3f\n%5.3f\n", config.time[0].delta, config.time[1].delta);
    }
    if(!log_csc_ts_result(&csc_sparse_mtx[0], stdout)) {
        fprintf(stderr, "Error logging transposed matrix to file.\n");
        return 0;
    }

    dealloc_csr(&csr_sparse_mtx, 1);
    dealloc_csc(&csc_sparse_mtx, 1);
    free(csr_sparse_mtx);
    free(csc_sparse_mtx);

    print(" ... transposing matrix.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int matrix_mp(void) {
    // -- SPARSE MATRIX REP CSR. --
    CSR *csr_sparse_mtx = malloc(2 * sizeof(CSR)); //structure containing coordinate format representation of matrix.
    CSC *csc_sparse_mtx = malloc(1 * sizeof(CSC));
    if((csc_sparse_mtx == NULL) || csr_sparse_mtx == NULL) {
        perror(NULL);
        return 0;
    }
    gettimeofday(&config.time[0].start, NULL);
    if(!read_to_csr(&csr_sparse_mtx, 0, 0)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    if(!read_to_csc(&csc_sparse_mtx, 0, 1)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    gettimeofday(&config.time[0].end, NULL);
    print_csr(&csr_sparse_mtx[0]);
    print_csc(&csc_sparse_mtx[0]);

    print(" ... Completed reading matrix into sparse matrix representations.");

    gettimeofday(&config.time[1].start, NULL);
    if(!process_multiplication(&csr_sparse_mtx[1], &csr_sparse_mtx[0], &csc_sparse_mtx[0])) {
        fprintf(stderr, "Error performing matrix multiplication on given matrix.\n");
        return 0;
    }
    gettimeofday(&config.time[1].end, NULL);

    print(" ... Completed matrix multiplication calculation.");

    if(!process_stat()) fprintf(stderr, "Error determining duration of operations.\n");

    print_csr(&csr_sparse_mtx[1]); //print resultant info

    // -- LOG RESULT TO FILE --
    if(config.log_fd != NULL) { //specified
        fprintf(config.log_fd, "%s\n", config.op_str);
        for(int i = 0; i < NUMBER_OF_INPUT_FILES; i++) {
            fprintf(config.log_fd, "%s\n", config.filename[i]);
        }
        fprintf(config.log_fd, "%i\n", config.num_threads);
        if(!log_csr_result(&csr_sparse_mtx[1], config.log_fd)) {
            fprintf(stderr, "Unable to log matrix result value.\n");
            return 0;
        }
        fprintf(config.log_fd, "%5.3f\n%5.3f\n", config.time[0].delta, config.time[1].delta);
    }

    if(!log_csr_result(&csr_sparse_mtx[1], stdout)) {
        fprintf(stderr, "Unable to log matrix result value.\n");
        return 0;
    }
    // -- DEALLOCATE --
    dealloc_csr(&csr_sparse_mtx, 2);
    dealloc_csc(&csc_sparse_mtx, 1);
    free(csc_sparse_mtx);
    free(csr_sparse_mtx);
    print(" ... performing matrix multiplication.");
    return 1;
}
