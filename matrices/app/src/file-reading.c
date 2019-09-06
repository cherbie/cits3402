#include "mop.h"

/**
 * Return 1 to indicate success and 0 to indicate failure.
 * @param k is the matrix index of the respective file
 */
int read_in_file(FILE* fp, int k) {
    int buffer_len = 10;
    char *buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    int count, col = 0;
    while(fgets(buffer, buffer_len, config.fd[k]) != NULL && count < 3) {
        switch (count) {
            case 0: {
                if(sscanf(buffer, "%s", coo_sparse_mtx[k].type) < 1) {
                    perror(NULL);
                    return 0;
                }
                break;
            }
            case 1: {
                if(fscanf(config.fd[k], "%i", &coo_sparse_mtx[k].row)) {
                    perror(NULL);
                    return 0;
                }
                break;
            }
            case 2: {
                if(fscanf(config.fd[k], "%i", &coo_sparse_mtx[k].col)) {
                    perror(NULL);
                    return 0;
                }
                break;
            }
            default: break;
        }
        count++;
    }
    col = coo_sparse_mtx[k].col;
    buffer_len = (col<<1) + 1; //accounting for spaces and NUL-byte char.
    buffer = realloc(buffer, buffer_len * sizeof(char));
    count = 0; //rows
    while(!feof(config.fd[k]) && count < coo_sparse_mtx[k].row) {
        if((fgets(buffer, buffer_len - 1, config.fd[k])) == NULL) break; // unexpected new-line encountered
        if(!read_in_mtx(&buffer, buffer_len, col, count, &k)) {
            fprintf(stderr, "Error interpreting given matrix values.\n");
            return 0;
        }
        count++;
    }
    return 1;
}

/**
 * @param k is the matrix index of the input file
 */
int read_in_mtx(char **buf, int len, int col, int row, const int *k) {
    int mtx_cols[col]; //array of column values.
    for(int i = 0; i < col; i++) {
        if(sprintf(*buf, "%i", mtx_cols[i]) < 1) {
            fprintf(stderr, "Error in read_in_mtx().\n");
            return 0;
        }
    }
    printf("Column matrix index (1) value = %i\n", mtx_cols[1]);
    return 1;
}
