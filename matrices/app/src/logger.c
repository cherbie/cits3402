#include "mop.h"

/**
 * Log the resulting matrix to stdout or file if specified.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int log_coo_result(COO *coo_mtx) {
    print(" --- ");
    int count = 0;
    for(int i = 0; i < (*coo_mtx).row; i++) {
        for(int j = 0; j < (*coo_mtx).col; j++) {
            if((count < (*coo_mtx).size) && (*coo_mtx).mtx[count][0] == i && (*coo_mtx).mtx[count][1] == j) {
                printf("%i ", (*coo_mtx).mtx[count++][2]);
            }
            else printf("%i ", 0);
        }
    }
    print("\n");
    print(" ---- ");
    return 1;
}
