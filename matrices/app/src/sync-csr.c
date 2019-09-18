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
     (*csc_mtx).col = (*csr_mtx).col;
     (*csc_mtx).row = (*csr_mtx).row;
     (*csc_mtx).size = (*csr_mtx).size;
     if((*csc_mtx).is_int) {
         (*csc_mtx).mtxi = malloc((*csr_mtx).size * sizeof(int));
         (*csc_mtx).mtx_offset = malloc(((*csr_mtx).col) * sizeof(int));
         (*csc_mtx).mtx_row = malloc((*csr_mtx).size * sizeof(int));
         if((*csc_mtx).mtxi == NULL || (*csc_mtx).mtx_offset == NULL || (*csc_mtx).mtx_row == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*csc_mtx).mtxi, (*csr_mtx).mtxi, (*csr_mtx).size * sizeof(int));
         memcpy((*csc_mtx).mtx_offset, (*csr_mtx).mtx_offset, (*csr_mtx).row * sizeof(int));
         memcpy((*csc_mtx).mtx_row, (*csr_mtx).mtx_col, (*csr_mtx).size * sizeof(int));
         return 1;
     }
     else {
         (*csc_mtx).mtxf = malloc((*csr_mtx).size * sizeof(float));
         (*csc_mtx).mtx_offset = malloc(((*csr_mtx).row) * sizeof(int));
         (*csc_mtx).mtx_row = malloc((*csr_mtx).size * sizeof(int));
         if((*csc_mtx).mtxf == NULL || (*csc_mtx).mtx_offset == NULL || (*csc_mtx).mtx_row == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*csc_mtx).mtxf, (*csr_mtx).mtxf, (*csr_mtx).size * sizeof(float));
         memcpy((*csc_mtx).mtx_offset, (*csr_mtx).mtx_offset, (*csr_mtx).row * sizeof(int));
         memcpy((*csc_mtx).mtx_row, (*csr_mtx).mtx_col, (*csr_mtx).size * sizeof(int));
         return 1;
     }
}

/**
 * Performs matrix multiplcation on given arguments 1 & 2.
 * Stores the resulting matrix in argument 0.
 * @param res_mtx CSC* resulting matrix stored in CSC struct.
 * @param csr_mtx CSR* the first matrix given.
 * @param csc_mtx CSC* the second matrix given.
 * @return 1 to indicate success and zero to indicate failure.
 */
int process_multiplication(CSC *res_mtx, CSR *csr_mtx, CSC *csc_mtx) {
    if((*csr_mtx).col != (*csc_mtx).row) {
        fprintf(stderr, "The dimensions of the provided matrices are not suitable.\n");
        return 0;
    }
    (*res_mtx).row = (*csr_mtx).row;
    (*res_mtx).col = (*csc_mtx).col;
    if(!(*csr_mtx).is_int || !(*csc_mtx).is_int) (*res_mtx).is_int = false;
    else (*res_mtx).is_int = true;
    int num_csr_nz, num_csc_nz, sum_csr_nz, sum_csc_nz, index_csr, index_csc, csr, csc, sum;
    num_csr_nz = 0; num_csc_nz = 0; sum_csr_nz = 0; sum_csc_nz = 0;
    if((*res_mtx).is_int) {
        (*res_mtx).mtxi = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int)); //maximum size
        (*res_mtx).mtx_offset = calloc((*res_mtx).col, sizeof(int));
        (*res_mtx).mtx_row = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxi == NULL || (*res_mtx).mtx_offset == NULL || (*res_mtx).mtx_row == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        (*res_mtx).size = 0;
        for(int i = 1; i <= (*csr_mtx).row; i++) {
            printf("row seen --- \n");
            num_csr_nz = (*csr_mtx).mtx_offset[i-1]; //need the convention at the start of the matrix
            sum_csr_nz += num_csr_nz - 1;
            sum_csc_nz = 0; //zer non-zero lookup index
            for(int j = 1; j <= (*csc_mtx).col; j++) {
                printf("col seen ---- \n");
                num_csc_nz = (*csc_mtx).mtx_offset[j-1];
                sum_csc_nz += num_csc_nz - 1;
                csr = 0; csc = 0; sum = 0; //row and column under inspection respectively
                if(num_csr_nz == 0 || num_csc_nz == 0) continue; // resultant offset = 0;
                for(int k = 0; k < (*csr_mtx).col; k++) {
                    if(csr >= num_csr_nz || csc >= num_csc_nz) continue; // can we use break here?
                    index_csr = sum_csr_nz + csr;
                    index_csc = sum_csc_nz + csc;
                    if((*csr_mtx).mtx_col[index_csr] == (*csc_mtx).mtx_row[index_csc]) { //nz element column matches row
                        sum += (*csr_mtx).mtxi[index_csr] * (*csc_mtx).mtxi[index_csc];
                        printf("Multiplication ==> %i x %i = #NUM\n", (*csr_mtx).mtxi[index_csr], (*csc_mtx).mtxi[index_csc]);
                    }
                    csr++; csc++;
                }
                //store sum;
                if(sum != 0) {
                    (*res_mtx).mtxi[(*res_mtx).size] = (*csr_mtx).mtxi[index_csr] * (*csc_mtx).mtxi[index_csc];
                    (*res_mtx).mtx_offset[j] += 1;
                    (*res_mtx).mtx_row[(*res_mtx).size] = csc; //column under inspection
                    (*res_mtx).size++;
                }
            }
        }
    }
    return 1;
}
