#include "mop.h"

/**
 * Performs scalar multiplication on COO sparse coordinate form matrix.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_scalar(COO *coo_mtx, float sc) {
    if((*coo_mtx).is_int) {
        for(int i = 0; i < (*coo_mtx).size; i++) (*coo_mtx).mtxi[i] *= sc;
    }
    else {
        for(int i = 0; i < (*coo_mtx).size; i++) (*coo_mtx).mtxf[i] *= sc;
    }
    return 1;
}

/**
 * Calculates the trace of the matrix
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_trace(COO *coo_mtx, int *i, float *f) {
    if((*coo_mtx).row != (*coo_mtx).col) {
        fprintf(stderr, "Error: the trace of a matrix is only well defined for square matrices.\n");
        return 0;
    }
    *i = 0; *f = 0.0;
    if((*coo_mtx).is_int) {
        for(int j = 0; j < (*coo_mtx).size; j++){
            if((*coo_mtx).rowcol[j][0] == (*coo_mtx).rowcol[j][1])
                (*i) += (*coo_mtx).mtxi[j];
        }
    }
    else {
        for(int j = 0; j < (*coo_mtx).size; j++){
            if((*coo_mtx).rowcol[j][0] == (*coo_mtx).rowcol[j][1])
                (*f) += (float) (*coo_mtx).mtxf[j];
        }
    }
    return 1;
}

/**
 * Performs matrix multiplication on the sparse matrices provided.
 * @return 1 to indicate success and 0 to indicate failure.

int process_addition(COO **coo_mtx) {
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
                    if(!add_float_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++; block2++;
                }
                else if(is_defined(&(*coo_mtx)[file1], i, j, block1)) {
                    val = (*coo_mtx)[file1].mtxf[block1][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_float_coo(&(*coo_mtx)[res], val, i, j, blockr)){
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++;
                }
                else if(is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    val = (*coo_mtx)[file2].mtxf[block2][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_float_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
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
                    if(!add_int_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++; block2++;
                }
                else if(is_defined(&(*coo_mtx)[file1], i, j, block1)) {
                    val = (*coo_mtx)[file1].mtxi[block1][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_int_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++;
                }
                else if(is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    val = (*coo_mtx)[file2].mtxi[block2][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_int_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block2++;
                }
            }
        }
    }
    return 1;
}
*/


/**
 * Performs matrix multiplication on the sparse matrices provided.
 * @return 1 to indicate success and 0 to indicate failure.
*/
int process_addition(CSR **csr_mtx) {
    CSR *res_mtx = &(*csr_mtx)[2];
    CSR *mtx_1 = &(*csr_mtx)[0];
    CSR *mtx_2 = &(*csr_mtx)[1];

    if(((*mtx_1).row != (*mtx_2).row) || ((*mtx_1).col != (*mtx_2).col)) {
        fprintf(stderr, "Error dimensions of the input matrices are not equivalent.\n");
        return 0;
    }
    else {
        (*res_mtx).row = (*mtx_1).row;
        (*res_mtx).col =  (*mtx_2).col;
    }
    bool set = false;
    int index_1, index_2, sum_nz_1, sum_nz_2, count_1, count_2;
    sum_nz_1 = 0; sum_nz_2 = 0;
    if(!(*mtx_1).is_int && !(*mtx_2).is_int) { //float
        float val = 0.0;
        (*res_mtx).is_int = false;
        (*res_mtx).mtxf = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(float)); //maximum size
        (*res_mtx).mtx_offset = calloc((*res_mtx).row+1, sizeof(int));
        (*res_mtx).mtx_col = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxf == NULL || (*res_mtx).mtx_offset == NULL || (*res_mtx).mtx_col == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        (*res_mtx).size = 0;
        sum_nz_1 = 0;
        sum_nz_2 = 0;
        for(int i = 0; i < (*mtx_1).row; i++) { //row of first & second matrix
            sum_nz_1 += (*mtx_1).mtx_offset[i];
            sum_nz_2 += (*mtx_2).mtx_offset[i];
            count_1 = 0;
            count_2 = 0;
            for(int j = 0; j < (*mtx_1).col; j++) { //columns of first and second matrix
                set = false;
                index_1 = sum_nz_1 + count_1;
                index_2 = sum_nz_2 + count_2;
                if((*mtx_1).mtx_col[index_1] == j && (*mtx_2).mtx_col[index_2] == j && count_1 >= (*mtx_1).mtx_offset[i+1] && count_2 >= (*mtx_2).mtx_offset[i+1]) {
                    val = (*mtx_1).mtxf[index_1] + (*mtx_2).mtxf[index_2];
                    set = true;
                    count_1++;
                    count_2++;
                }
                else if((*mtx_1).mtx_col[index_1] == j && count_1 >= (*mtx_1).mtx_offset[i+1]) {
                    val = (*mtx_1).mtxf[index_1];
                    set = true;
                    count_1++;
                }
                else if((*mtx_2).mtx_col[index_2] == j && count_2 >= (*mtx_2).mtx_offset[i+1]) {
                    val = (*mtx_2).mtxf[index_2];
                    set = true;
                    count_2++;
                }
                if(set) {
                    (*res_mtx).mtxf[(*res_mtx).size] = val;
                    (*res_mtx).mtx_offset[i+1] += 1;
                    (*res_mtx).mtx_col[(*res_mtx).size] = j;
                    (*res_mtx).size += 1;
                }
            }
        }
    }
    else {
        int val = 0;
        (*res_mtx).is_int = true;
        (*res_mtx).mtxi = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int)); //maximum size
        (*res_mtx).mtx_offset = calloc((*res_mtx).row+1, sizeof(int));
        (*res_mtx).mtx_col = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxi == NULL || (*res_mtx).mtx_offset == NULL || (*res_mtx).mtx_col == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        (*res_mtx).size = 0;
        sum_nz_1 = 0;
        sum_nz_2 = 0;
        for(int i = 0; i < (*mtx_1).row; i++) { //row of first & second matrix
            sum_nz_1 += (*mtx_1).mtx_offset[i];
            sum_nz_2 += (*mtx_2).mtx_offset[i];
            count_1 = 0;
            count_2 = 0;
            for(int j = 0; j < (*mtx_1).col; j++) { //columns of first and second matrix
                set = false;
                index_1 = sum_nz_1 + count_1;
                index_2 = sum_nz_2 + count_2;
                if((*mtx_1).mtx_col[index_1] == j && (*mtx_2).mtx_col[index_2] == j && count_1 < (*mtx_1).mtx_offset[i+1] && count_2 < (*mtx_2).mtx_offset[i+1]) {
                    val = (*mtx_1).mtxi[index_1] + (*mtx_2).mtxi[index_2];
                    set = true;
                    count_1++;
                    count_2++;
                }
                else if((*mtx_1).mtx_col[index_1] == j && count_1 < (*mtx_1).mtx_offset[i+1]) {
                    val = (*mtx_1).mtxi[index_1];
                    set = true;
                    count_1++;
                }
                else if((*mtx_2).mtx_col[index_2] == j && count_2 < (*mtx_2).mtx_offset[i+1]) {
                    val = (*mtx_2).mtxi[index_2];
                    set = true;
                    count_2++;
                }
                if(set) {
                    (*res_mtx).mtxi[(*res_mtx).size] = val;
                    (*res_mtx).mtx_offset[i+1] += 1;
                    (*res_mtx).mtx_col[(*res_mtx).size] = j;
                    (*res_mtx).size += 1;
                }
            }
        }
    }
    return 1;
}
