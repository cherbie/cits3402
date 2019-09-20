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
         (*csc_mtx).mtx_offset = malloc(((*csr_mtx).col + 1) * sizeof(int));
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
         (*csc_mtx).mtx_offset = malloc(((*csr_mtx).row + 1) * sizeof(int));
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
int process_multiplication(CSR *res_mtx, CSR *mtx_1, CSR *mtx_2) {
    if((*mtx_1).col != (*mtx_2).row) {
        fprintf(stderr, "The dimensions of the provided matrices are not suitable.\n");
        return 0;
    }
    (*res_mtx).row = (*mtx_1).row;
    (*res_mtx).col = (*mtx_2).col;
    if(!(*mtx_1).is_int || !(*mtx_2).is_int) (*res_mtx).is_int = false;
    else (*res_mtx).is_int = true;
    int num_nz_1, num_nz_2, sum_nz_1, sum_nz_2, index_1, index_2, one, two;
    num_nz_1 = 0; num_nz_2 = 0; sum_nz_1 = 0; sum_nz_2 = 0;
    bool *seen_2 = calloc(((*res_mtx).row * (*res_mtx).col), sizeof(bool));
    bool *seen_1 = calloc(((*mtx_1).col), sizeof(bool));
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
        for(int i = 1; i <= (*mtx_1).row; i++) {
            printf("row seen --- \n");
            sum_nz_1 += (*mtx_1).mtx_offset[i-1]; //add previous amount of non-zero elements. starting index in nnz array
            num_nz_1 = (*mtx_1).mtx_offset[i]; //current amount of non-zero elements
            seen_2 = calloc(((*res_mtx).row * (*res_mtx).col), sizeof(bool));
            for(int j = 1; j <= (*mtx_2).row; j++) { //represent rows  of  2nd matrix searched
                printf("col seen ---- \n");
                seen_1 = calloc(((*mtx_1).col), sizeof(bool));
                sum_nz_2 = 0; //zero non-zero lookup index
                num_nz_2 = (*mtx_2).mtx_offset[j]; //number of non-zero's in current row
                one = 0; two = 0; sum = 0; //row and column under inspection respectively
                if(num_nz_1 == 0 || num_nz_2 == 0) continue; // resultant offset = 0;
                for(int k = 1; k <= (*mtx_2).row; k++) { //represent columns of resultant matrix.
                    sum_nz_2 += (*mtx_2).mtx_offset[k-1]; //number of  non-zero's before current row
                    index_2 = sum_nz_2;
                    int count = 1;
                    while(seen_2[index_2]) {
                        count++;
                        index_2++;
                    }
                    if(count > num_nz_2) continue;
                    count = 1;
                    index_1 = sum_nz_1;  //row and column under inspection in first mtx
                    while(seen_1[index_1]) {
                        count++;
                        index_1++;
                    }
                    if(count > num_nz_1) continue;
                    printf(" elem ---> %i & %i\n",(k-1), (j-1));
                    printf(" index ---> %i-%i || %i-%i\n", sum_nz_1, index_1, sum_nz_2, index_2);
                    printf(" col ---> %i & %i\n", (*mtx_1).mtx_col[index_1], (*mtx_2).mtx_col[index_2]);
                    if((*mtx_1).mtx_col[index_1] != (k-1)) continue;
                    //else one++;
                    if((*mtx_2).mtx_col[index_2] != (j-1)) continue;
                    //else two++;
                    //if(two >= num_nz_2 || one >= num_nz_1) break;
                    //if((*mtx_1).mtx_col[index_1] == (*mtx_2).mtx_col[index_2]) { //nz element column matches row
                    sum += (*mtx_1).mtxi[index_1] * (*mtx_2).mtxi[index_2];
                    printf("Multiplication ==> %i x %i ==> %i\n", (*mtx_1).mtxi[index_1], (*mtx_2).mtxi[index_2], sum);
                    seen_2[index_2] = true;
                    seen_1[index_1] = true;
                    //}

                }
                //store sum;
                if(sum != 0) {
                    (*res_mtx).mtxi[(*res_mtx).size] = sum;
                    (*res_mtx).mtx_offset[j] += 1;
                    (*res_mtx).mtx_col[(*res_mtx).size] = (j-1); //column under inspection
                    (*res_mtx).size++;
                }
            }
        }
    }
    else {
        float sum;
        (*res_mtx).mtxf = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(float)); //maximum size
        (*res_mtx).mtx_offset = calloc((*res_mtx).col, sizeof(int));
        (*res_mtx).mtx_col = malloc(((*res_mtx).row * (*res_mtx).col) * sizeof(int));
        if((*res_mtx).mtxf == NULL || (*res_mtx).mtx_offset == NULL || (*res_mtx).mtx_col == NULL) {
            perror("function: process_multiplication().");
            return 0;
        }
        (*res_mtx).size = 0;
        for(int i = 1; i <= (*mtx_1).row; i++) {
            printf("row seen --- \n");
            sum_nz_1 += (*mtx_1).mtx_offset[i-1];
            num_nz_1 = (*mtx_1).mtx_offset[i];
            for(int j = 1; j <= (*mtx_2).col; j++) {
                printf("col seen ---- \n");
                sum_nz_2 = 0; //zer non-zero lookup index
                sum_nz_2 += (*mtx_2).mtx_offset[j-1];
                num_nz_2 = (*mtx_2).mtx_offset[j];
                one = 0; two = 0; sum = 0.0; //row and column under inspection respectively
                if(num_nz_1 == 0 || num_nz_1 == 0) continue; // resultant offset = 0;
                for(int k = 0; k < (*mtx_2).row; k++) {
                    index_1 = (sum_nz_1) + one;
                    index_2 = sum_nz_2 + two;
                    printf(" ---> %i & %i\n",index_1, index_2);
                    printf(" ---> %i & %i\n", (*mtx_1).mtx_col[index_1], (*mtx_2).mtx_col[index_2]);
                    if((*mtx_1).mtx_col[index_1] == (*mtx_2).mtx_col[index_2]) { //nz element column matches row
                        sum += (*mtx_1).mtxf[index_1] * (*mtx_2).mtxf[index_2];
                        printf("Multiplication ==> %f x %f = %f\n", (*mtx_1).mtxf[index_1], (*mtx_2).mtxf[index_2], sum);
                    }
                    if((*mtx_1).mtx_col[index_1] == (j-1)) one++;
                    if((*mtx_2).mtx_col[index_2] == (j-1)) two++;
                    if(two >= num_nz_2 || one >= num_nz_1) break;
                }
                //store sum;
                if(sum != 0) {
                    (*res_mtx).mtxf[(*res_mtx).size] = sum;
                    (*res_mtx).mtx_offset[j] += 1;
                    (*res_mtx).mtx_col[(*res_mtx).size] = j-1; //column under inspection
                    (*res_mtx).size++;
                }
            }
        }
    }
    return 1;
}
