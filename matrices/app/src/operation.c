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
    const int num_coo = 1;
    int file_id = 0;
    int coo_id = 0;
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_coo * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_coo(&coo_sparse_mtx, coo_id, file_id)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    if(!process_scalar(&coo_sparse_mtx[coo_id], config.sm)) {
        fprintf(stderr, "Error performing scalar multiplication.\n");
        return 0;
    }

    print_coo(&coo_sparse_mtx[coo_id]);

    if(!log_coo_result(&(coo_sparse_mtx[coo_id]))) {
        fprintf(stderr, "Error logging result to file.\n");
        return 0;
    }

    dealloc_coo(&coo_sparse_mtx, num_coo);
    free(coo_sparse_mtx);
    print(" ... completed scalar multiplication & deallocated memory");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int trace(void) {
    const int num_coo = 1;
    int coo_id = 0;
    int file_id = 0;
    //SPARSE MATRIX REP.
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_coo * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_coo(&coo_sparse_mtx, coo_id, file_id)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    int i;
    float f;

    if(!process_trace(&coo_sparse_mtx[coo_id], &i, &f)) {
        fprintf(stderr, "Error calulating the trace of the matrix provided.\n");
        return 0;
    }
    if(!log_trace_result(&coo_sparse_mtx[coo_id], &i, &f)) {
        fprintf(stderr, "Error logging the result of the trace.\n");
        return 0;
    }
    dealloc_coo(&coo_sparse_mtx, num_coo);
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
    for(int k = 0; k < NUMBER_OF_INPUT_FILES; k++) {
        if(!read_to_coo(&coo_sparse_mtx, coo_id++, k)) {
            fprintf(stderr, "Error converting file to sparse matrix form.\n");
            return 0;
        }
    }
    if(!process_addition(&coo_sparse_mtx)) {
        fprintf(stderr, "Error performing matrix addition.\n");
        return 0;
    }

    print_coo(&(coo_sparse_mtx[2]));

    if(!log_coo_result(&(coo_sparse_mtx[coo_id]))) {
        fprintf(stderr, "Error logging result to file.\n");
        return 0;
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
    const int num_csr = 1;
    const int num_csc = 1;
    int csr_id = 0;
    int csc_id = 0;
    int file_id = 0;
    //SPARSE MATRIX REP CSR.
    CSR *csr_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((csr_sparse_mtx = malloc(num_csr * sizeof(CSR))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_csr(&csr_sparse_mtx, csr_id, file_id++)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    CSC *csc_sparse_mtx;
    if((csc_sparse_mtx = malloc(num_csc * sizeof(CSC))) == NULL) {
        perror(NULL);
        return 0;
    }
    print_csr(&csr_sparse_mtx[0]); //debugging
    print("FINISHED READING CSR FILE\n");
    if(!process_transpose(&csr_sparse_mtx[csr_id], &csc_sparse_mtx[csc_id])) {
        fprintf(stderr, "Error transposing given matrix.\n");
        return 0;
    }

    print_csc(&csc_sparse_mtx[csc_id]);

    printf("memcompare = %i\n", memcmp(&csr_sparse_mtx[csr_id].mtxi, &csc_sparse_mtx[csc_id].mtxi, csc_sparse_mtx[csc_id].size * sizeof(int)));

    if(!log_csc_ts_result(&(csc_sparse_mtx)[csc_id])) {
        fprintf(stderr, "Error logging transposed matrix to file.\n");
        return 0;
    }

    dealloc_csr(&csr_sparse_mtx, num_csr);
    dealloc_csc(&csc_sparse_mtx, num_csc);
    free(csr_sparse_mtx);
    free(csc_sparse_mtx);

    print(" ... transposing matrix.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int matrix_mp(void) {
    const int num_csr = 1;
    const int num_csc = 2;
    int csr_id = 0;
    int csc_id = 0;
    int file_id = 0;
    //SPARSE MATRIX REP CSR.
    CSR *csr_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((csr_sparse_mtx = malloc(num_csr * sizeof(CSR))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_csr(&csr_sparse_mtx, csr_id, file_id++)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    print_csr(&csr_sparse_mtx[csr_id]); //debugging

    CSC *csc_sparse_mtx;
    if((csc_sparse_mtx = malloc(num_csc * sizeof(CSC))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_csc(&csc_sparse_mtx, csc_id, file_id)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    print_csc(&csc_sparse_mtx[csc_id]);
    print(" ... Completed reading matrix into sparse matrix representations.");

    if(!process_multiplication(&csc_sparse_mtx[1], &csr_sparse_mtx[0], &csc_sparse_mtx[0])) {
        fprintf(stderr, "Error performing matrix multiplication on given matrix.\n");
        return 0;
    }

    print(" ... Completed matrix multiplication calculation.");

    dealloc_csr(&csr_sparse_mtx, num_csr);
    dealloc_csc(&csc_sparse_mtx, num_csc);
    free(csr_sparse_mtx);
    free(csc_sparse_mtx);
    print(" ... performing matrix multiplication.");
    return 1;
}
