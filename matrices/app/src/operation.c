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
    print(" ... performing scalar multiplication.");
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
