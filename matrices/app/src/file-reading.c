#include "mop.h"

/**
 * Return 1 to indicate success and 0 to indicate failure.
 * @param k is the matrix index of the respective file
 */
int read_file_main(COO **sparse_mtx, int k) {
    int buffer_len = 10;
    char *buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        (*sparse_mtx)[k].type = strdup(buffer);
        (*sparse_mtx)[k].type = str_clean((*sparse_mtx)[k].type);
        if((*sparse_mtx)[k].type == NULL) {
            perror(NULL);
            return 0;
        }
    }
    buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        buffer = str_clean(buffer);
        (*sparse_mtx)[k].row = atoi(buffer);
        if((*sparse_mtx)[k].row == 0) {
            perror(NULL);
            return 0;
        }
    }
    buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        buffer = str_clean(buffer);
        (*sparse_mtx)[k].col = atoi(buffer);
        if((*sparse_mtx)[k].col == 0) {
            perror(NULL);
            return 0;
        }
    }
    buffer = calloc(buffer_len, sizeof(char)); //store column values for each row entry
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(strcmp((*sparse_mtx)[k].type, "int") == 0) {
        if(!read_coo_file(sparse_mtx, k)) return 0;
    }
    else if(strcmp((*sparse_mtx)[k].type, "float") == 0) {
        if(!read_float_file(&buffer, k)) return 0;
    }
    else {
        fprintf(stderr, "Incorrect variable type specified in file.\n");
        fclose(config.fd[k]);
        free(buffer);
        return 0;
    }

    fclose(config.fd[k]);
    free(buffer);

    print_coo(&(*sparse_mtx)[k]);
    return 1;
}

int read_float_file(char **buf, int k) {
    return 1;
}

/** ONLY IMPLEMENTED WITH INT ATM
 * @param k is the matrix index of the file underquestion.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int read_coo_file(COO **coo_mtx, int k) {
    int rows, cols, c, dsize, val;
    rows = 0, cols = 0; c = 0; dsize = 0; val = 0;
    int len = 6; //accounting for spaces and NUL-byte char.
    bool content = false;
    char *buf = calloc(len, sizeof(char)); //store column values for each row entry
    if(buf == NULL) {
        perror(NULL);
        return 0;
    }
    (*coo_mtx)[k].size = 0; //no elements
    (*coo_mtx)[k].mtx = malloc(1 * sizeof(int *));
    while((c = fgetc(config.fd[k])) != EOF) {
        if(iscntrl(c) && content) {
            (*coo_mtx)[k].size += 1;
            (*coo_mtx)[k].mtx = realloc((*coo_mtx)[k].mtx, (*coo_mtx)[k].size * sizeof(int *));
            if((*coo_mtx)[k].mtx == NULL) {
                perror("function: read_int_file()");
                return 0;
            }
            val = atoi(buf);
            if(!add_int_coo(coo_mtx, k, val, rows, cols, (*coo_mtx)[k].size - 1)) {
                perror(NULL);
                return 0;
            }
            content = false;
            break;
        }
        else if(iscntrl(c)) break;
        else if(isdigit(c) && c != '0') { //CURRENTLY ONLY READING INTEGERS
            if(dsize >= len) {
                len += 1;
                buf = realloc(buf, len + 1 * sizeof(char));
                if(buf == NULL) {
                    perror("function: read_int_file()");
                    return 0;
                }
            }
            buf[dsize++] = (char) c;
            buf[dsize++] = '\0';
            content = true;
            continue;
        }
        else if(isspace(c)) {
            (*coo_mtx)[k].size += 1;
            (*coo_mtx)[k].mtx = realloc((*coo_mtx)[k].mtx, (*coo_mtx)[k].size * sizeof(int *));
            if((*coo_mtx)[k].mtx == NULL) {
                perror("function: read_int_file()");
                return 0;
            }
            val = atoi(buf);
            if(!add_int_coo(coo_mtx, k, val, rows, cols, (*coo_mtx)[k].size - 1)) {
                perror(NULL);
                return 0;
            }
            cols++;
            if(cols >= (*coo_mtx)[k].col) { //next row
                rows++;
                cols = 0;
            }
            dsize = 0;
            content = false;
        }
        buf = calloc(len, sizeof(char));
    }
    free(buf);
    return 1;
}

/**
 * Dynamically creates Sparse Coordinate matrix of input file.
 * @param k is the application file identifier.
 * @count is the array index of the coordinates.
 */
int add_int_coo(COO **coo_mtx, int k, int val, int row, int col, int index) {
    (*coo_mtx)[k].mtx[index] = malloc(3  * sizeof(int));
    if((*coo_mtx)[k].mtx == NULL) {
        perror("function: add_int_coo()");
        return 0;
    }
    (*coo_mtx)[k].mtx[index][0] = row;
    (*coo_mtx)[k].mtx[index][1] = col;
    (*coo_mtx)[k].mtx[index][2] = val;
    return 1;
}
