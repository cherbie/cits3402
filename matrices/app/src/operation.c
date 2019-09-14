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
    const int num_files = 1;
    int file_id = 0;
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_files * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_coo(&coo_sparse_mtx, file_id)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    if(!process_scalar(&coo_sparse_mtx[file_id], config.sm)) {
        fprintf(stderr, "Error performing scalar multiplication.\n");
        return 0;
    }

    print_coo(&coo_sparse_mtx[file_id]);

    if(!log_coo_result(&(coo_sparse_mtx[file_id]))) {
        fprintf(stderr, "Error logging result to file.\n");
        return 0;
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
    int file_id = 0;
    //SPARSE MATRIX REP.
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_files * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_coo(&coo_sparse_mtx, file_id)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    int i;
    float f;

    if(!process_trace(&coo_sparse_mtx[file_id], &i, &f)) {
        fprintf(stderr, "Error calulating the trace of the matrix provided.\n");
        return 0;
    }
    if(!log_trace_result(&coo_sparse_mtx[file_id], &i, &f)) {
        fprintf(stderr, "Error logging the result of the trace.\n");
        return 0;
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
    const int num_struct = 3;
    //SPARSE MATRIX REP.
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(num_struct * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    for(int k = 0; k < NUMBER_OF_INPUT_FILES; k++) {
        if(!read_to_coo(&coo_sparse_mtx, k)) {
            fprintf(stderr, "Error converting file to sparse matrix form.\n");
            return 0;
        }
    }
    if(!process_addition(&coo_sparse_mtx)) {
        fprintf(stderr, "Error performing matrix addition.\n");
        return 0;
    }

    print_coo(&(coo_sparse_mtx[num_struct - 1]));

    if(!log_coo_result(&(coo_sparse_mtx[num_struct - 1]))) {
        fprintf(stderr, "Error logging result to file.\n");
        return 0;
    }

    dealloc_coo(&coo_sparse_mtx, num_struct);
    free(coo_sparse_mtx);
    print(" ... performing matrix addition.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int transpose_matrix(void) {
    const int num_files = 1;
    int file_id = 0;
    //SPARSE MATRIX REP CSR.
    CSR *csr_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((csr_sparse_mtx = malloc(num_files * sizeof(CSR))) == NULL) {
        perror(NULL);
        return 0;
    }
    if(!read_to_csr(&csr_sparse_mtx, file_id)) {
        fprintf(stderr, "Error converting file to sparse matrix form.\n");
        return 0;
    }
    CSS *css_sparse_mtx;
    if((css_sparse_mtx = malloc(num_files * sizeof(CSR))) == NULL) {
        perror(NULL);
        return 0;
    }

    print("FINISHED READING CSR FILE\n");

    if(!process_transpose(&csr_sparse_mtx[file_id], &css_sparse_mtx[file_id])) {
        fprintf(stderr, "Error transposing given matrix.\n");
        return 0;
    }

    dealloc_csr(&csr_sparse_mtx, num_files);
    dealloc_css(&css_sparse_mtx, num_files);
    free(csr_sparse_mtx);
    free(css_sparse_mtx);

    print(" ... transposing matrix.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int matrix_mp(void) {
    print(" ... performing matrix multiplication.");
    return 1;
}
