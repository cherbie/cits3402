#include "mop.h"

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
    int num_nz_1, num_nz_2, sum_nz_1, sum_nz_2, count_1, count_2;
    num_nz_1 = 0; num_nz_2 = 0, sum_nz_1 = 0; sum_nz_2 = 0;
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
            print(" -- row seen -- ");
            if((*mtx_1).mtx_offset[i+1] == 0) continue;
            //num_nz_1 = (*mtx_1).mtx_offset[i+1];
            sum_nz_1 += (*mtx_1).mtx_offset[i];
            sum_nz_2 = 0;
            //num_nz_2 = 0;
            for(int j = 0; j < (*mtx_2).col; j++) { //column of 2nd matrix
                print(" -- column seen -- ");
                if((*mtx_2).mtx_offset[j+1] == 0) continue;
                //num_nz_2 = (*mtx_2).mtx_offset[j+1];
                sum_nz_2 += (*mtx_2).mtx_offset[j];
                num_nz_2 = sum_nz_2;
                num_nz_1 = sum_nz_1;
                sum = 0;
                count_1 = 0;
                count_2 = 0;
                for(int k = 0; k < (*mtx_2).row; k++) { //row of 2nd matrix
                    if(count_1 >= (*mtx_1).mtx_offset[i+1] || count_2 >= (*mtx_2).mtx_offset[j+1]) break;
                    num_nz_1 += count_1;
                    num_nz_2 += count_2;
                    printf(" elem ---> %i & %i\n", k, j);
                    printf(" col ---> %i & %i\n", (*mtx_1).mtx_col[num_nz_1], (*mtx_2).mtx_row[num_nz_2]);
                    if((*mtx_1).mtx_col[num_nz_1] == k && (*mtx_2).mtx_row[num_nz_2] == k) {
                        sum += (*mtx_1).mtxi[num_nz_1] * (*mtx_2).mtxi[num_nz_2];
                        printf(" index ---> %i x %i => sum = %i\n", (*mtx_1).mtxi[num_nz_1], (*mtx_2).mtxi[num_nz_2], sum);
                        count_1++;
                        count_2++;
                    }
                    else if((*mtx_1).mtx_col[num_nz_1] == k) count_1++;
                    else if((*mtx_2).mtx_row[num_nz_2] == k) count_2++;
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
            print(" -- row seen -- ");
            if((*mtx_1).mtx_offset[i+1] == 0) continue;
            //num_nz_1 = (*mtx_1).mtx_offset[i+1];
            sum_nz_1 += (*mtx_1).mtx_offset[i];
            sum_nz_2 = 0;
            //num_nz_2 = 0;
            for(int j = 0; j < (*mtx_2).col; j++) { //column of 2nd matrix
                print(" -- column seen -- ");
                if((*mtx_2).mtx_offset[j+1] == 0) continue;
                //num_nz_2 = (*mtx_2).mtx_offset[j+1];
                sum_nz_2 += (*mtx_2).mtx_offset[j];
                num_nz_2 = sum_nz_2;
                num_nz_1 = sum_nz_1;
                sum = 0;
                count_1 = 0;
                count_2 = 0;
                for(int k = 0; k < (*mtx_2).row; k++) { //row of 2nd matrix
                    if(count_1 >= (*mtx_1).mtx_offset[i+1] || count_2 >= (*mtx_2).mtx_offset[j+1]) break;
                    num_nz_1 += count_1;
                    num_nz_2 += count_2;
                    printf(" elem ---> %i & %i\n", k, j);
                    printf(" col ---> %i & %i\n", (*mtx_1).mtx_col[num_nz_1], (*mtx_2).mtx_row[num_nz_2]);
                    if((*mtx_1).mtx_col[num_nz_1] == k && (*mtx_2).mtx_row[num_nz_2] == k) {
                        sum += (*mtx_1).mtxf[num_nz_1] * (*mtx_2).mtxf[num_nz_2];
                        printf(" index ---> %3.2f x %3.2f => sum = %3.2f\n", (*mtx_1).mtxf[num_nz_1], (*mtx_2).mtxf[num_nz_2], sum);
                        count_1++;
                        count_2++;
                    }
                    else if((*mtx_1).mtx_col[num_nz_1] == k) count_1++;
                    else if((*mtx_2).mtx_row[num_nz_2] == k) count_2++;
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
