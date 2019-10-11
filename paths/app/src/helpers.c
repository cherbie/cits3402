#include "sp.h"

/**
 * Print to standard out the matrix specified.
**/
void print_matrix(int ***mtx, int *size) {
    for(int i = 0; i < (*size);  i++) {
        for(int j = 0; j < (*size); j++) {
            printf("%i ", (*mtx)[i][j]);
        }
        printf("\n");
    }
}
