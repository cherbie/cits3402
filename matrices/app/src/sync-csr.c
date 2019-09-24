#include "mop.h"

/**
 * Performs matrix multiplication on the sparse matrices provided.
 * @return 1 to indicate success and 0 to indicate failure.
*/
int process_addition(CSR **csr_mtx, COO **coo_mtx) {
    COO *res_mtx = &(*coo_mtx)[0];
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
        (*res_mtx).size = 0;
        (*res_mtx).mtxf = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(float *)); //maximum size
        //(*res_mtx).mtx_offset = calloc((*res_mtx).row+1, sizeof(int));
        //(*res_mtx).mtx_col = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxf == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        sum_nz_1 = 0; sum_nz_2 = 0;
        for(int i = 0; i < (*mtx_1).row; i++) {
            sum_nz_1 += (*mtx_1).mtx_offset[i];
            sum_nz_2 += (*mtx_2).mtx_offset[i];
            count_1 = 0; count_2 = 0;
            for(int j = 0; j < (*mtx_1).col; j++) {
                set = false;
                index_1 = sum_nz_1 + count_1;
                index_2 = sum_nz_2 + count_2;
                if((*mtx_1).mtx_col[index_1] == j && (*mtx_2).mtx_col[index_2] == j && count_1 < (*mtx_1).mtx_offset[i+1] && count_2 < (*mtx_2).mtx_offset[i+1]) {
                    val = (*mtx_1).mtxf[index_1] + (*mtx_2).mtxf[index_2];
                    set = true;
                    count_1++;
                    count_2++;
                }
                else if((*mtx_1).mtx_col[index_1] == j && count_1 < (*mtx_1).mtx_offset[i+1]) {
                    val = (*mtx_1).mtxf[index_1];
                    set = true;
                    count_1++;
                }
                else if((*mtx_2).mtx_col[index_2] == j && count_2 < (*mtx_2).mtx_offset[i+1]) {
                    val = (*mtx_2).mtxf[index_2];
                    set = true;
                    count_2++;
                }
                if(set) {
                    (*res_mtx).mtxf[(*res_mtx).size] = malloc(3 * sizeof(float));
                    (*res_mtx).mtxf[(*res_mtx).size][0] = (float) i;
                    (*res_mtx).mtxf[(*res_mtx).size][1] = (float) j;
                    (*res_mtx).mtxf[(*res_mtx).size][2] = val;
                    (*res_mtx).size += 1;
                }
            }
        }
    }
    else {
        int val = 0;
        (*res_mtx).is_int = true;
        (*res_mtx).size = 0;
        (*res_mtx).mtxi = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int *)); //maximum size
        //(*res_mtx).mtx_offset = calloc((*res_mtx).row+1, sizeof(int));
        //(*res_mtx).mtx_col = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxi == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        sum_nz_1 = 0; sum_nz_2 = 0;
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
                    (*res_mtx).mtxi[(*res_mtx).size] = malloc(3 * sizeof(int));
                    if((*res_mtx).mtxi[(*res_mtx).size] == NULL) {
                        perror(NULL);
                        return 0;
                    }
                    (*res_mtx).mtxi[(*res_mtx).size][0] = i;
                    (*res_mtx).mtxi[(*res_mtx).size][1] = j;
                    (*res_mtx).mtxi[(*res_mtx).size][2] = val;
                    (*res_mtx).size += 1;
                }
            }
        }
    }
    return 1;
}


/**
 * Copies the CSR struct meta data to the CSC struct meta data.
 * @param csr_mtx typedef struct CSR
 * @param csc_mtx typedef struct CSC
 */
int process_transpose(CSR *csr_mtx, CSC *csc_mtx) {
    /*
     * Need to allocate to:
     *  - csc_mtx.mtxi & csc_mtx.mtxf
     *  - csc_mtx.mtx_offset
     *  - csc_mtx.mtx_col
     */
     (*csc_mtx).is_int = (*csr_mtx).is_int;
     (*csc_mtx).col = (*csr_mtx).row;
     (*csc_mtx).row = (*csr_mtx).col;
     (*csc_mtx).size = (*csr_mtx).size;
     if((*csc_mtx).is_int) {
         (*csc_mtx).mtxi = malloc((*csr_mtx).size * sizeof(int));
         (*csc_mtx).mtx_offset = malloc(((*csc_mtx).col + 1) * sizeof(int));
         (*csc_mtx).mtx_row = malloc((*csr_mtx).size * sizeof(int));
         if((*csc_mtx).mtxi == NULL || (*csc_mtx).mtx_offset == NULL || (*csc_mtx).mtx_row == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*csc_mtx).mtxi, (*csr_mtx).mtxi, (*csr_mtx).size * sizeof(int));
         memcpy((*csc_mtx).mtx_offset, (*csr_mtx).mtx_offset, ((*csc_mtx).col+1) * sizeof(int));
         memcpy((*csc_mtx).mtx_row, (*csr_mtx).mtx_col, (*csr_mtx).size * sizeof(int));
         return 1;
     }
     else {
         (*csc_mtx).mtxf = malloc((*csr_mtx).size * sizeof(float));
         (*csc_mtx).mtx_offset = malloc(((*csc_mtx).col + 1) * sizeof(int));
         (*csc_mtx).mtx_row = malloc((*csr_mtx).size * sizeof(int));
         if((*csc_mtx).mtxf == NULL || (*csc_mtx).mtx_offset == NULL || (*csc_mtx).mtx_row == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*csc_mtx).mtxf, (*csr_mtx).mtxf, (*csr_mtx).size * sizeof(float));
         memcpy((*csc_mtx).mtx_offset, (*csr_mtx).mtx_offset, ((*csc_mtx).col+1) * sizeof(int));
         memcpy((*csc_mtx).mtx_row, (*csr_mtx).mtx_col, (*csr_mtx).size * sizeof(int));
         return 1;
     }
}

/**
 * Performs matrix multiplcation on given arguments 1 & 2.
 * Stores the resulting matrix in argument 0.
 * @param res_mtx CSC* resulting matrix stored in CSC struct.
 * @param mtx_1 CSR* the first matrix given.
 * @param mtx_2 CSC* the second matrix given.
 * @return 1 to indicate success and zero to indicate failure.
 */
int process_multiplication(CSR *res_mtx, CSR *mtx_1, CSC *mtx_2) {
    if((*mtx_1).col != (*mtx_2).row) {
        fprintf(stderr, "The dimensions of the provided matrices are not suitable.\n");
        return 0;
    }
    (*res_mtx).row = (*mtx_1).row;
    (*res_mtx).col = (*mtx_2).col;
    if(!(*mtx_1).is_int || !(*mtx_2).is_int) (*res_mtx).is_int = false;
    else (*res_mtx).is_int = true;
    int index_1, index_2, sum_nz_1, sum_nz_2, count_1, count_2;
    sum_nz_1 = 0; sum_nz_2 = 0;
    if((*res_mtx).is_int) {
        int sum;
        (*res_mtx).mtxi = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int)); //maximum size
        (*res_mtx).mtx_offset = calloc((*res_mtx).row+1, sizeof(int));
        (*res_mtx).mtx_col = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxi == NULL || (*res_mtx).mtx_offset == NULL || (*res_mtx).mtx_col == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        (*res_mtx).size = 0;
        sum_nz_1 = 0;
        for(int i = 0; i < (*mtx_1).row; i++) { //row of first matrix
            if((*mtx_1).mtx_offset[i+1] == 0) continue;
            sum_nz_1 += (*mtx_1).mtx_offset[i];
            sum_nz_2 = 0;
            for(int j = 0; j < (*mtx_2).col; j++) { //column of 2nd matrix
                if((*mtx_2).mtx_offset[j+1] == 0) continue;
                sum_nz_2 += (*mtx_2).mtx_offset[j];
                sum = 0; count_1 = 0; count_2 = 0;
                for(int k = 0; k < (*mtx_2).row; k++) { //row of 2nd matrix
                    if(count_1 >= (*mtx_1).mtx_offset[i+1] || count_2 >= (*mtx_2).mtx_offset[j+1]) break;
                    index_1 = sum_nz_1 + count_1;
                    index_2 = sum_nz_2 + count_2;
                    if((*mtx_1).mtx_col[index_1] == k && (*mtx_2).mtx_row[index_2] == k) {
                        sum += (*mtx_1).mtxi[index_1] * (*mtx_2).mtxi[index_2];
                        count_1++;
                        count_2++;
                    }
                    else if((*mtx_1).mtx_col[index_1] == k) count_1++;
                    else if((*mtx_2).mtx_row[index_2] == k) count_2++;
                }
                if(sum != 0) {
                    (*res_mtx).mtxi[(*res_mtx).size] = sum;
                    (*res_mtx).mtx_offset[i+1] += 1;
                    (*res_mtx).mtx_col[(*res_mtx).size] = j; //column under inspection
                    (*res_mtx).size += 1;
                }
            }
        }
    }
    else {
        float sum;
        (*res_mtx).mtxf = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(float)); //maximum size
        (*res_mtx).mtx_offset = calloc((*res_mtx).row+1, sizeof(int));
        (*res_mtx).mtx_col = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxf == NULL || (*res_mtx).mtx_offset == NULL || (*res_mtx).mtx_col == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        (*res_mtx).size = 0;
        sum_nz_1 = 0;
        for(int i = 0; i < (*mtx_1).row; i++) { //row of first matrix
            if((*mtx_1).mtx_offset[i+1] == 0) continue;
            sum_nz_1 += (*mtx_1).mtx_offset[i];
            sum_nz_2 = 0;
            for(int j = 0; j < (*mtx_2).col; j++) { //column of 2nd matrix
                if((*mtx_2).mtx_offset[j+1] == 0) continue;
                sum_nz_2 += (*mtx_2).mtx_offset[j];
                sum = 0; count_1 = 0; count_2 = 0;
                for(int k = 0; k < (*mtx_2).row; k++) { //row of 2nd matrix
                    if(count_1 >= (*mtx_1).mtx_offset[i+1] || count_2 >= (*mtx_2).mtx_offset[j+1]) break;
                    index_1 = sum_nz_1 + count_1;
                    index_2 = sum_nz_2 + count_2;
                    if((*mtx_1).mtx_col[index_1] == k && (*mtx_2).mtx_row[index_2] == k) {
                        sum += (*mtx_1).mtxf[index_1] * (*mtx_2).mtxf[index_2];
                        count_1++;
                        count_2++;
                    }
                    else if((*mtx_1).mtx_col[index_1] == k) count_1++;
                    else if((*mtx_2).mtx_row[index_2] == k) count_2++;
                }
                if(sum != 0) {
                    (*res_mtx).mtxf[(*res_mtx).size] = sum;
                    (*res_mtx).mtx_offset[i+1] += 1;
                    (*res_mtx).mtx_col[(*res_mtx).size] = j; //column under inspection
                    (*res_mtx).size += 1;
                }
            }
        }
    }
    return 1;
}
