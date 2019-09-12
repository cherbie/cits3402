#include "mop.h"

/**
 * Deallocate dynamic memory within COO sparse matrix rep. data structure.
 * @param size is the number of input files with meta data dynamically allocated.
 */
void dealloc_coo(COO **coo_mtx, int size) {
    for(int k = 0; k < size; k++) {
        if((*coo_mtx)[k].is_int) {
            for(int i = 0; i < (*coo_mtx)[k].size; i++) {
                free((*coo_mtx)[k].mtxi[i]);
            }
            free((*coo_mtx)[k].mtxi);
        }
        else {
            for(int i = 0; i < (*coo_mtx)[k].size; i++) {
                free((*coo_mtx)[k].mtxf[i]);
            }
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
    for(int i = 0; i < NUMBER_OF_INPUT_FILES; i++) {
        free(config.filename[i]); //char pointers in filenames
    }
    free(config.fd); //array of file descriptors
    free(config.filename); //array of filenames
}
