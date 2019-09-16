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
 * @param csr_mtx type CSR
 */
int log_csr_result(CSR *csr_mtx) {
    int nz, row_el;
    print(" --- ");
    nz = 0; // index of current nnz element
    if((*csr_mtx).is_int) {
    	for(int i = 0; i < (*csr_mtx).row; i++) { //rows
            row_el = (*csr_mtx).mtx_offset[i];
            for(int j = 0; j < (*csr_mtx).col; j++) {
                if((*csr_mtx).mtx_offset[i] == 0) printf("%i ", 0);
                else if(row_el > 0) { // nnz elements left to be 'consumed'
                    if((*csr_mtx).mtx_col[nz] == j) { //the column of the nz element
                        printf("%i ", (*csr_mtx).mtxi[nz++]);
                        row_el--;
                    }
                    else { //No non-zero element for this column
                        printf("%i ", 0);
                    }
              	}
                else {
                    printf("%i ", 0);
                }
           }
        }
    }
    else {
    	for(int i = 0; i < (*csr_mtx).row; i++) { //rows
            row_el = (*csr_mtx).mtx_offset[i];
            for(int j = 0; j < (*csr_mtx).col; j++) {
                if((*csr_mtx).mtx_offset[i] == 0) printf("%3.2f ", 0.0);
                else if(row_el > 0) { // nnz elements left to be 'consumed'
                    if((*csr_mtx).mtx_col[nz] == j) { //the column of the nz element
                        printf("%3.2f ", (*csr_mtx).mtxf[nz++]);
                        row_el--;
                    }
                    else { //No non-zero element for this column
                        printf("%3.2f ", 0.0);
                    }
              	}
                else {
                    printf("%3.2f ", 0.0);
                }
           }
        }

    }
    print("\n");
    print(" --- ");
    return 1;
}

/**
 * Log the resulting matrix to stdout or file if specified.
 * @param csc_mtx type CSC*
 */
int log_csc_result(CSC * csc_mtx) {
    int nz, row_el;
    print(" --- ");
    nz = 0; // index of current nnz element
    if((*csc_mtx).is_int) {
    	for(int i = 0; i < (*csc_mtx).row; i++) { //rows
            row_el = (*csc_mtx).mtx_offset[i];
            for(int j = 0; j < (*csc_mtx).col; j++) {
                if((*csc_mtx).mtx_offset[i] == 0) printf("%i ", 0);
                else if(row_el > 0) { // nnz elements left to be 'consumed'
                    if((*csc_mtx).mtx_col[nz] == j) { //the column of the nz element
                        printf("%i ", (*csc_mtx).mtxi[nz++]);
                        row_el--;
                    }
                    else { //No non-zero element for this column
                        printf("%i ", 0);
                    }
              	}
                else {
                    printf("%i ", 0);
                }
           }
        }
    }
    return 1;
}
