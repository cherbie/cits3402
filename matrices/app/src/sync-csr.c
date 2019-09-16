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
         (*csc_mtx).mtxi = malloc((*csc_mtx).size * sizeof(int));
         (*csc_mtx).mtx_offset = malloc(((*csc_mtx).row) * sizeof(int));
         (*csc_mtx).mtx_col = malloc((*csc_mtx).size * sizeof(int));
         if((*csc_mtx).mtxi == NULL || (*csc_mtx).mtx_offset == NULL || (*csc_mtx).mtx_col == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*csc_mtx).mtxi, (*csr_mtx).mtxi, (*csc_mtx).size * sizeof(int));
         memcpy((*csc_mtx).mtx_offset, (*csr_mtx).mtx_offset, (*csc_mtx).row * sizeof(int));
         memcpy((*csc_mtx).mtx_col, (*csr_mtx).mtx_col, (*csc_mtx).size * sizeof(int));
         return 1;
     }
     else {
         (*csc_mtx).mtxf = malloc((*csc_mtx).size * sizeof(float));
         (*csc_mtx).mtx_offset = malloc(((*csc_mtx).row) * sizeof(int));
         (*csc_mtx).mtx_col = malloc((*csc_mtx).size * sizeof(int));
         if((*csc_mtx).mtxf == NULL || (*csc_mtx).mtx_offset == NULL || (*csc_mtx).mtx_col == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*csc_mtx).mtxf, (*csr_mtx).mtxf, (*csc_mtx).size * sizeof(float));
         memcpy((*csc_mtx).mtx_offset, (*csr_mtx).mtx_offset, (*csc_mtx).row * sizeof(int));
         memcpy((*csc_mtx).mtx_col, (*csr_mtx).mtx_col, (*csc_mtx).size * sizeof(int));
         return 1;
     }
}
