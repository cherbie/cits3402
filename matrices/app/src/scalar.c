#include "mop.h"

/**
 * Performs scalar multiplication on COO sparse coordinate form matrix.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_scalar_mp(COO *coo_mtx, float sc) {
    for(int i = 0; i < (*coo_mtx).size; i++){
        (*coo_mtx).mtx[i][2] *= sc;
    }
    return 1;
}
