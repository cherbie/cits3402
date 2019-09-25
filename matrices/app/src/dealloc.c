#include "mop.h"

/**
 * Deallocate dynamic memory within COO sparse matrix rep. data structure.
 * @param size is the number of input files with meta data dynamically allocated.
 */
void dealloc_coo(COO **coo_mtx, int size) {
    for(int k = 0; k < size; k++) {
        if((*coo_mtx)[k].is_int) {
            for(int i = 0; i < (*coo_mtx)[k].size; i++) free((*coo_mtx)[k].mtxi[i]);
            free((*coo_mtx)[k].mtxi);
        }
        else {
            for(int i = 0; i < (*coo_mtx)[k].size; i++) free((*coo_mtx)[k].mtxf[i]);
            free((*coo_mtx)[k].mtxf);
        }
    }
}

/**
 * Dealloc CONFIG structure fields.
 * Note: log files are deallocated in seperate function.
 */
void dealloc_config(void) {
    free(config.op_str); //char pointers
    for(int i = 0; i < config.num_files; i++) {
        free(config.filename[i]); //char pointers in filenames
        fclose(config.fd[i]);
    }
    free(config.fd); //array of file descriptors
    free(config.filename); //array of filenames
    free(config.time);
    free(config.exec_time); //not malloced?
    fclose(config.log_fd);
    free(config.log_filename);
}

/**
 * Deallocate dynamic memory within CSR sparse matrix rep. data structure.
 * @param size is the number of input files with meta data dynamically allocated.
 */
void dealloc_csr(CSR **csr_mtx, int size) {
    for(int k = 0; k < size; k++) {
        if((*csr_mtx)[k].is_int) free((*csr_mtx)[k].mtxi);
        else free((*csr_mtx)[k].mtxf);
        free((*csr_mtx)[k].mtx_offset);
        free((*csr_mtx)[k].mtx_col);
    }
}

/**
 * Deallocate dynamic memory within CCS sparse matrix rep. data structure.
 * @param size is the number of input files with meta data dynamically allocated.
 */
void dealloc_csc(CSC **csc_mtx, int size) {
    for(int k = 0; k < size; k++) {
        if((*csc_mtx)[k].is_int) free((*csc_mtx)[k].mtxi);
        else free((*csc_mtx)[k].mtxf);
        free((*csc_mtx)[k].mtx_offset);
        free((*csc_mtx)[k].mtx_row);
    }
}
