#include "sp.h"

/**
 * Print to standard out the matrix specified.
**/
void print_matrix(int **mtx, int *size) {
    for(int i = 0; i < (*size);  i++) {
        for(int j = 0; j < (*size); j++) {
            printf("%i ", mtx[i][j]);
        }
        printf("\n");
    }
}

/**
 * Dynamically allocate memory for a matrix.
 * @return the pointer to the array or NULL on error.
**/
int **create_matrix(int *size) {
    int **mtx = malloc((*size) * sizeof(int *));
    if(mtx == NULL) {
        perror(NULL);
        return NULL;
    }
    for(int i = 0; i < (*size); i++) {
        mtx[i] = calloc((*size), sizeof(int));
        if(mtx[i] == NULL) {
            perror(NULL);
            return NULL;
        }
    }
    return mtx;
}

/**
 * @return 0 to indicate sucess and -1 to indicate failure.
 */
int prep_weights(int **weights, int *size) {
    for(int i = 0; i < (*size); i++) {
        for(int j = 0; j < (*size); j++) {
            if(weights[i][j] == 0) weights[i][j] = LARGE_INT; // set high weight
            else continue;
        }
    }
    return 0;
}

/**
 * Duplicates a given matrix
 * @return 0 to indicate success and -1 to  indicate failure.
 */
int dup_matrix(int **dest, int **orig, int *size) {
    for(int i = 0; i < (*size); i++) {
        dest[i] = malloc((*size) * sizeof(int));
        if(dest[i] == NULL) {
            perror(NULL);
            return -1;
        }
        memcpy(&dest[i][0], &orig[i][0], (*size) * sizeof(int));
    }
    return 0;
}

/**
 * Copy the contents of an existing matrix.
**/
int cpy_matrix(int **dest, int **orig, int *size) {
    for(int i = 0; i < (*size); i++) memcpy(dest[i], orig[i], (*size) * sizeof(int));
    return 0;
}
