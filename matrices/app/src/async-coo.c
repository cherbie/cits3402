#include "mop.h"

/**
 * Performs scalar multiplication on COO sparse coordinate form matrix.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_scalar_async(COO *coo_mtx, float sc) {
    if((*coo_mtx).is_int) {
#pragma omp parallel for
        for(int i = 0; i < (*coo_mtx).size; i++) {
            (*coo_mtx).mtxi[i] *= sc;
        }
    }
    else {
#pragma omp parallel for
        for(int i = 0; i < (*coo_mtx).size; i++) {
            (*coo_mtx).mtxf[i] *= sc;
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
            if((*coo_mtx).rowcol[j][0] == (*coo_mtx).rowcol[j][1])
                element += (*coo_mtx).mtxi[j];
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
            if((*coo_mtx).rowcol[j][0] == (*coo_mtx).rowcol[j][1])
                element += (*coo_mtx).mtxf[j];
        }
#pragma omp single
        {
            (*f) = element;
        }
    }
    return 1;
}

/**
 * Performs matrix multiplication on the sparse matrices provided.
 * @return 1 to indicate success and 0 to indicate failure.

int process_addition_async(COO **coo_mtx) {
    const int file1 = 0;
    const int file2 = 1;
    const int res = 2;
    long int block1, block2, blockr;

    if(((*coo_mtx)[file1].row != (*coo_mtx)[file2].row) || ((*coo_mtx)[file1].col != (*coo_mtx)[file2].col)) {
        fprintf(stderr, "Error dimensions of the input matrices are not equivalent.\n");
        return 0;
    }
    else {
        (*coo_mtx)[res].row = (*coo_mtx)[file1].row;
        (*coo_mtx)[res].col =  (*coo_mtx)[file1].col;
    }
    block1 = 0; block2 = 0; blockr = 0;
    if(!(*coo_mtx)[file1].is_int && !(*coo_mtx)[file2].is_int) { //float
        float val = 0.0;
        (*coo_mtx)[res].is_int = false;
        (*coo_mtx)[res].mtxf = malloc(((*coo_mtx)[file1].size + (*coo_mtx)[file2].size) * sizeof(float *)); //initial maximum allocation
        if((*coo_mtx)[res].mtxf == NULL) {
            perror("Error allocating memory for resultant sparse matrix.");
            return 0;
        }
        for(int i = 0; i < (*coo_mtx)[file1].row; i++) {
            if(((*coo_mtx)[file1].mtxf[block1][0] != (float) i) && ((*coo_mtx)[file2].mtxf[block2][0] != (float) i)) continue;
            for(int j = 0; j < (*coo_mtx)[file1].col; j++) {
                if(is_defined(&(*coo_mtx)[file1], i, j, block1) && is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    val = (*coo_mtx)[file1].mtxf[block1][2] + (*coo_mtx)[file2].mtxf[block2][2];
                    (*coo_mtx)[res].size += 1;
                    (*coo_mtx)[res].mtxf[blockr] = malloc(3  * sizeof(float));
                    if((*coo_mtx)[res].mtxf[blockr] == NULL) {
                        perror("function: add_int_coo()");
                        return 0;
                    }
                    #pragma omp parallel shared(block1, block2, blockr)
                    {
                    #pragma omp single
                    {
                    #pragma omp task firstprivate(blockr, i, j, val)
                    {
                        (*coo_mtx)[res].mtxf[blockr][0] = i;
                        (*coo_mtx)[res].mtxf[blockr][1] = j;
                        (*coo_mtx)[res].mtxf[blockr][2] = val;
                    }
                    }
                    }
                    blockr++; block1++; block2++;
                }
                else if(is_defined(&(*coo_mtx)[file1], i, j, block1)) {
                    val = (*coo_mtx)[file1].mtxf[block1][2];
                    (*coo_mtx)[res].size += 1;
                    (*coo_mtx)[res].mtxf[blockr] = malloc(3  * sizeof(float));
                    if((*coo_mtx)[res].mtxf[blockr] == NULL) {
                        perror("function: add_int_coo()");
                        return 0;
                    }
                    #pragma omp parallel shared(block1, block2, blockr)
                    {
                    #pragma omp single
                    {
                    #pragma omp task firstprivate(blockr, i, j, val)
                    {
                        (*coo_mtx)[res].mtxf[blockr][0] = i;
                        (*coo_mtx)[res].mtxf[blockr][1] = j;
                        (*coo_mtx)[res].mtxf[blockr][2] = val;
                    }
                    }
                    }
                    blockr++; block1++;
                }
                else if(is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    val = (*coo_mtx)[file2].mtxf[block2][2];
                    (*coo_mtx)[res].size += 1;
                    (*coo_mtx)[res].mtxf[blockr] = malloc(3  * sizeof(float));
                    if((*coo_mtx)[res].mtxf[blockr] == NULL) {
                        perror("function: add_int_coo()");
                        return 0;
                    }
                    #pragma omp parallel shared(block1, block2, blockr)
                    {
                    #pragma omp single
                    {
                    #pragma omp task firstprivate(blockr, i, j, val)
                    {
                        (*coo_mtx)[res].mtxf[blockr][0] = i;
                        (*coo_mtx)[res].mtxf[blockr][1] = j;
                        (*coo_mtx)[res].mtxf[blockr][2] = val;
                    }
                    }
                    }
                    blockr++; block2++;
                }
                continue;
            }
        }
    }
    else { //int
        int val = 0;
        (*coo_mtx)[res].is_int = true;
        (*coo_mtx)[res].mtxi = malloc(((*coo_mtx)[file1].size + (*coo_mtx)[file2].size) * sizeof(int *)); //initial maximum allocation
        if((*coo_mtx)[res].mtxi == NULL) {
            perror("Error allocating memory for resultant sparse matrix.");
            return 0;
        }
        for(int i = 0; i < (*coo_mtx)[file1].row; i++) {
            if((*coo_mtx)[file1].mtxi[block1][0] != i && (*coo_mtx)[file2].mtxi[block2][0] != i) continue;
            for(int j = 0; j < (*coo_mtx)[file1].col; j++) {
                if(is_defined(&(*coo_mtx)[file1], i, j, block1) && is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    val = (*coo_mtx)[file1].mtxi[block1][2] + (*coo_mtx)[file2].mtxi[block2][2];
                    (*coo_mtx)[res].size += 1;
                    (*coo_mtx)[res].mtxi[blockr] = malloc(3  * sizeof(int));
                    if((*coo_mtx)[res].mtxi[blockr] == NULL) {
                        perror("function: add_int_coo()");
                        return 0;
                    }
                    #pragma omp parallel
                    {
                    #pragma omp single
                    {
                    #pragma omp task firstprivate(blockr, i, j, val)
                    {
                        (*coo_mtx)[res].mtxi[blockr][0] = i;
                        (*coo_mtx)[res].mtxi[blockr][1] = j;
                        (*coo_mtx)[res].mtxi[blockr][2] = val;
                    }
                    }
                    }
                    blockr++; block1++; block2++;
                }
                else if(is_defined(&(*coo_mtx)[file1], i, j, block1)) {
                    val = (*coo_mtx)[file1].mtxi[block1][2];
                    (*coo_mtx)[res].size += 1;
                    (*coo_mtx)[res].mtxi[blockr] = malloc(3  * sizeof(int));
                    if((*coo_mtx)[res].mtxi[blockr] == NULL) {
                        perror("function: add_int_coo()");
                        return 0;
                    }
                    #pragma omp parallel
                    {
                    #pragma omp single
                    {
                    #pragma omp task firstprivate(blockr, i, j, val)
                    {
                        (*coo_mtx)[res].mtxi[blockr][0] = i;
                        (*coo_mtx)[res].mtxi[blockr][1] = j;
                        (*coo_mtx)[res].mtxi[blockr][2] = val;
                    }
                    }
                    }
                    blockr++; block1++;
                }
                else if(is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    val = (*coo_mtx)[file2].mtxi[block2][2];
                    (*coo_mtx)[res].size += 1;
                    (*coo_mtx)[res].mtxi[blockr] = malloc(3  * sizeof(int));
                    if((*coo_mtx)[res].mtxi[blockr] == NULL) {
                        perror("function: add_int_coo()");
                        return 0;
                    }
                    #pragma omp parallel
                    {
                    #pragma omp single
                    {
                    #pragma omp task firstprivate(blockr, i, j, val)
                    {
                        (*coo_mtx)[res].mtxi[blockr][0] = i;
                        (*coo_mtx)[res].mtxi[blockr][1] = j;
                        (*coo_mtx)[res].mtxi[blockr][2] = val;
                    }
                    }
                    }
                    blockr++; block2++;
                }
            }
        }
    }
    return 1;
}

*/
