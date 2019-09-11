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
int scalar_mp(void) {
    //SPARSE MATRIX REP.
    COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
    if((coo_sparse_mtx = malloc(NUMBER_OF_INPUT_FILES * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    int file_id = 0;
    read_file_main(&coo_sparse_mtx, file_id);
    if(!process_scalar_mp(&coo_sparse_mtx[file_id], config.sm)) {
        fprintf(stderr, "Error performing scalar multiplication.\n");
        return 0;
    }

    print_coo(&coo_sparse_mtx[file_id]);

    if(!log_coo_result(&(coo_sparse_mtx[file_id]))) {
        fprintf(stderr, "Error logging result to file.\n");
        return 0;
    }

    print(" ... completed scalar multiplication.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int trace(void) {
    print(" ... finding trace of matrix.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int addition(void) {
    print(" ... performing matrix addition.");
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int transpose_matrix(void) {
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
