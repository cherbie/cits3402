#include "mop.h"

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int read_in_file(FILE* fp, int file_num) {
    if(realloc(coo_sparse_mtx, file_num * sizeof(COO)) == NULL) {
        perror(NULL);
        return 0;
    }
    int k = file_num - 1;
    int buffer_len = 10;
    char *buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    int count, col, ch = 0;
    while(fgets(buffer, buffer_len, config.in1_fd) != NULL && count < 3) {
        switch (count) {
            case 0: {
                if(sscanf(buffer, "%s", coo_sparse_mtx[k].type) < 1) {
                    perror(NULL);
                    return 0;
                }
                break;
            }
            case 1: {
                if(fscanf(config.in1_fd, "%i", &coo_sparse_mtx[k].row)) {
                    perror(NULL);
                    return 0;
                }
                break;
            }
            case 2: {
                if(fscanf(config.in1_fd, "%i", &coo_sparse_mtx[k].col)) {
                    perror(NULL);
                    return 0;
                }
                break;
            }
            default: break;
        }
        count++;
    }
    buffer_len = config.col<<1 + 1; //accounting for spaces and NUL-byte char.
    buffer = realloc(buffer, buffer_len * sizeof(char));
    count = 0;
    while(!feof(config.in1_fd) && count < config.row) {
        if((fgets(buffer, buffer_len - 1, config.in1_fd)) == NULL) break; // unexpected new-line encountered
        if(!read_in_mtx(*buffer, buffer_len, count)) {
            fprintf(stderr, "Error interpreting given matrix values.\n");
            return 0;
        }
    }
}

int read_in_mtx(char **buf, int *len, int *row) {
    int mtx_cols[config.col] = {0};
    for(int i = 0; i < config.col; i++) {
        if(sprintf(*buf, "%i", mtx_cols[i]) < 1) {
            fprintf(stderr, "Error in read_in_mtx().\n");
            exit(EXIT_FAILURE);
        }
    }
}
