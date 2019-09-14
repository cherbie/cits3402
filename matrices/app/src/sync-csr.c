#include "mop.h"

/**
 * Copies the CSR struct meta data to the CSS struct meta data.
 * @param csr_mtx typedef struct CSR
 * @param css_mtx typedef struct CSS
 */
int process_transpose(CSR * csr_mtx, CSS * css_mtx) {
    /*
     * Need to allocate to:
     *  - css_mtx.mtxi & css_mtx.mtxf
     *  - css_mtx.mtx_offset
     *  - css_mtx.mtx_col
     */
     (*css_mtx).is_int = (*csr_mtx).is_int;
     (*css_mtx).col = (*csr_mtx).col;
     (*css_mtx).row = (*csr_mtx).row;
     (*css_mtx).size = (*csr_mtx).size;
     int n = (*css_mtx).row * (*css_mtx).col; //number of elements in matrix
     if((*css_mtx).is_int) {
         (*css_mtx).mtxi = malloc((*css_mtx).size * sizeof(int));
         (*css_mtx).mtx_offset = malloc((n+1) * sizeof(int));
         (*css_mtx).mtx_col = malloc((*css_mtx).size * sizeof(int));
         if((*css_mtx).mtxi == NULL || (*css_mtx).mtx_offset == NULL || (*css_mtx).mtx_col == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*css_mtx).mtxi, (*csr_mtx).mtxi, (*css_mtx).size);
         memcpy((*css_mtx).mtx_offset, (*csr_mtx).mtx_offset, (n+1));
         memcpy((*css_mtx).mtx_col, (*csr_mtx).mtx_col, (*css_mtx).size);
         return 1;
     }
     else {
         (*css_mtx).mtxf = malloc((*css_mtx).size * sizeof(float));
         (*css_mtx).mtx_offset = malloc((n+1) * sizeof(int));
         (*css_mtx).mtx_col = malloc((*css_mtx).size * sizeof(int));
         if((*css_mtx).mtxf == NULL || (*css_mtx).mtx_offset == NULL || (*css_mtx).mtx_col == NULL) {
             perror("function: process_transpose()");
             return 0;
         }
         memcpy((*css_mtx).mtxf, (*csr_mtx).mtxf, (*css_mtx).size);
         memcpy((*css_mtx).mtx_offset, (*csr_mtx).mtx_offset, (n+1));
         memcpy((*css_mtx).mtx_col, (*csr_mtx).mtx_col, (*css_mtx).size);
         return 1;
     }
}
