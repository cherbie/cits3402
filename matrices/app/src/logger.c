#include "mop.h"

/**
 * Log the resulting matrix to stdout or file if specified.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int log_coo_result(COO *coo_mtx) {
    print(" --- ");
    int count = 0;
    if((*coo_mtx).is_int) {
        for(int i = 0; i < (*coo_mtx).row; i++) {
            for(int j = 0; j < (*coo_mtx).col; j++) {
                if((count < (*coo_mtx).size) && (*coo_mtx).mtxi[count][0] == i && (*coo_mtx).mtxi[count][1] == j) {
                    printf("%i ", (*coo_mtx).mtxi[count++][2]);
                }
                else printf("%i ", 0);
            }
        }
    }
    else {
        for(int i = 0; i < (*coo_mtx).row; i++) {
            for(int j = 0; j < (*coo_mtx).col; j++) {
                if((count < (*coo_mtx).size) && (*coo_mtx).mtxf[count][0] == i && (*coo_mtx).mtxf[count][1] == j) {
                    printf("%3.2f ", (*coo_mtx).mtxf[count++][2]);
                }
                else printf("%3.2f ", (float) 0.0);
            }
        }
    }
    print("\n");
    print(" ---- ");
    return 1;
}

int log_trace_result(COO *coo_mtx, int *tracei, float *tracef) {
    print(" --- ");
    if((*coo_mtx).is_int) printf("%i", *tracei);
    else printf("%3.2f", *tracef);
    printf("\n");
    print(" --- ");
    return 1;
}

/**
 * Log the resulting matrix to stdout or file if specified.
 * @param css_mtx type CSS
 */
int log_transpose_result(CSS * css_mtx) {
    return 1;
}
