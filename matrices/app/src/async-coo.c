#include "mop.h"

/**
 * Performs scalar multiplication on COO sparse coordinate form matrix.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_scalar_async(COO *coo_mtx, float sc) {
    if((*coo_mtx).is_int) {
        #pragma omp parallel for
        for(int i = 0; i < (*coo_mtx).size; i++) {
            (*coo_mtx).mtxi[i][2] *= sc;
        }
    }
    else {
        #pragma omp parallel for
        for(int i = 0; i < (*coo_mtx).size; i++) {
             (*coo_mtx).mtxf[i][2] *= sc;
        }
    }
    return 1;
}

/**
 * Calculates the trace of the matrix
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_trace_async(COO *coo_mtx, int *i, float *f) {
    if((*coo_mtx).row != (*coo_mtx).col) {
        fprintf(stderr, "Error: the trace of a matrix is only well defined for square matrices.\n");
        return 0;
    }
    int size = (*coo_mtx).size;
    if((*coo_mtx).is_int) {
        int element = 0;
        #pragma omp parallel for reduction(+:element)
        for(int j = 0; j < size; j++) {
            if((*coo_mtx).mtxi[j][0] == (*coo_mtx).mtxi[j][1])
                element += (*coo_mtx).mtxi[j][2];
        }
        #pragma omp single
        {
            (*i) = element;
        }
    }
    else {
        float element = 0.0;
        #pragma omp parallel for reduction(+:element)
        for(int j = 0; j < size; j++) {
            if((*coo_mtx).mtxf[j][0] == (*coo_mtx).mtxf[j][1])
                element += (*coo_mtx).mtxf[j][2];
        }
        #pragma omp single
        {
            (*f) = element;
        }
    }
    return 1;
}
