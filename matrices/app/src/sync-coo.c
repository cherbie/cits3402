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

/**
 * Performs matrix multiplication on the sparse matrices provided.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_addition(COO **coo_mtx) {
    const int file1 = 0;
    const int file2 = 1;
    const int res = 2;
    int block1, block2, blockr;

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
        print("stage1");
        for(int i = 0; i < (*coo_mtx)[file1].row; i++) {
            if((*coo_mtx)[file1].mtxf[block1][0] != (float) i && (*coo_mtx)[file2].mtxf[block2][0] != (float) i) continue;
            print("stage2");
            for(int j = 0; j < (*coo_mtx)[file1].col; j++) {
                print("stage3");
                if(is_defined(&(*coo_mtx)[file1], i, j, block1) && is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    print("stage4");
                    val = (*coo_mtx)[file1].mtxf[block1][2] + (*coo_mtx)[file2].mtxf[block2][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_float_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++; block2++;
                    print("stage5");
                }
                else if(is_defined(&(*coo_mtx)[file1], i, j, block1)) {
                    print("stage6");
                    val = (*coo_mtx)[file1].mtxf[block1][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_float_coo(&(*coo_mtx)[res], val, i, j, blockr)){
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++;
                    print("stage7");
                }
                else if(is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    print("stage8");
                    val = (*coo_mtx)[file2].mtxf[block2][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_float_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block2++;
                    print("stage9");
                }
                print("stage10");
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
        print("stage1");
        for(int i = 0; i < (*coo_mtx)[file1].row; i++) {
            if((*coo_mtx)[file1].mtxi[block1][0] != i && (*coo_mtx)[file2].mtxi[block2][0] != i) continue;
            print("stage2");
            for(int j = 0; j < (*coo_mtx)[file1].col; j++) {
                print("stage3");
                if(is_defined(&(*coo_mtx)[file1], i, j, block1) && is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    print("stage4");
                    val = (*coo_mtx)[file1].mtxi[block1][2] + (*coo_mtx)[file2].mtxi[block2][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_int_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++; block2++;
                    print("stage5");
                }
                else if(is_defined(&(*coo_mtx)[file1], i, j, block1)) {
                    print("stage6");
                    val = (*coo_mtx)[file1].mtxi[block1][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_int_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block1++;
                    print("stage7");
                }
                else if(is_defined(&(*coo_mtx)[file2], i, j, block2)) {
                    print("stage8");
                    val = (*coo_mtx)[file2].mtxi[block2][2];
                    (*coo_mtx)[res].size += 1;
                    if(!add_int_coo(&(*coo_mtx)[res], val, i, j, blockr)) {
                        fprintf(stderr, "Error allocating matrix addition to memory space.\n");
                        return 0;
                    }
                    blockr++; block2++;
                    print("stage9");
                }
                print("stage11");
                continue;
            }
        }
    }
    print("addition complete\n");

    return 1;
}
