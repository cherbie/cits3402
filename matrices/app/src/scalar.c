#include "mop.h"

/**
 * Performs scalar multiplication on COO sparse coordinate form matrix.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_scalar(COO *coo_mtx, float sc) {
    if((*coo_mtx).is_int) {
        for(int i = 0; i < (*coo_mtx).size; i++){
            (*coo_mtx).mtxi[i][2] *= sc;
        }
    }
    else {
        for(int i = 0; i < (*coo_mtx).size; i++){
            (*coo_mtx).mtxf[i][2] *= sc;
        }
    }
    return 1;
}

/**
 * Calculates the trace of the matrix
 */
int process_trace(COO *coo_mtx, int *i, float *f) {
    *i = 0; *f = 0.0;
    if((*coo_mtx).is_int) {
        for(int j = 0; j < (*coo_mtx).size; j++){
            if((*coo_mtx).mtxi[j][0] == (*coo_mtx).mtxi[j][1])
                (*i) += (*coo_mtx).mtxi[j][2];
        }
    }
    else {
        for(int j = 0; j < (*coo_mtx).size; j++){
            if((*coo_mtx).mtxf[j][0] == (*coo_mtx).mtxf[j][1])
                (*f) += (float) (*coo_mtx).mtxf[j][2];
        }
    }
    return 1;
}
