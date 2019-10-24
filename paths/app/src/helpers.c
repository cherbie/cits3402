#include "sp.h"

/**
 * Print to standard out the matrix specified.
**/
void print_matrix(int *mtx, int *size) {
    int rowoffset;
    for(int i = 0; i < (*size);  i++) {
        rowoffset = i * (*size); // start index of row
        for(int j = 0; j < (*size); j++) {
            printf("%i ", mtx[rowoffset + j]);
        }
        printf("\n");
    }
}
/**
 * @return 0 to indicate sucess and -1 to indicate failure.
 */
int prep_weights(int *weights, int *size) {
    int rowoffset;
    for(int i = 0; i < (*size); i++) {
        rowoffset = i * (*size); // start index of row
        for(int j = 0; j < (*size); j++) {
            if(weights[rowoffset + j] == 0) weights[rowoffset + j] = LARGE_INT; // set high weight
            if(i == j) weights[rowoffset + j] = LARGE_INT;
            else continue;
        }
    }
    return 0;
}
