#include "mop.h"

/**
 * Return 1 to indicate success and 0 to indicate failure.
 * @param k is the matrix index of the respective file
 */
int read_file_main(int k) {
    int buffer_len = 10;
    print("b");
    char *buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        print("c");
        coo_sparse_mtx[k].type = strdup(buffer);
        coo_sparse_mtx[k].type = str_clean(coo_sparse_mtx[k].type);
        if(coo_sparse_mtx[k].type == NULL) {
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
        print("d");
        buffer = str_clean(buffer);
        coo_sparse_mtx[k].row = atoi(buffer);
        if(coo_sparse_mtx[k].row == 0) {
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
        print("e");
        buffer = str_clean(buffer);
        coo_sparse_mtx[k].col = atoi(buffer);
        if(coo_sparse_mtx[k].col == 0) {
            perror(NULL);
            return 0;
        }
    }
    buffer = calloc(buffer_len, sizeof(char)); //store column values for each row entry
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    print("h");
    if(strcmp(coo_sparse_mtx[k].type, "int") == 0) {
        if(!read_int_file(k)) return 0;
    }
    else if(strcmp(coo_sparse_mtx[k].type, "float") == 0) {
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
    print_coo(k);
    return 1;
}

int read_float_file(char **buf, int k) {
    return 1;
}

/**
 * @param k is the matrix index of the file underquestion.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int read_int_file(int k) {
    int rows = 0;
    int cols = 0;
    int c = 0;
    int dsize = 0;
    int val = 0;
    int len = 6; //accounting for spaces and NUL-byte char.
    print("k");
    char *buf = calloc(len, sizeof(char)); //store column values for each row entry
    if(buf == NULL) {
        perror(NULL);
        return 0;
    }
    print("h");
    coo_sparse_mtx[k].size = 0; //no elements
    coo_sparse_mtx[k].mtx = malloc(1 * sizeof(int *));
    while((c = fgetc(config.fd[k])) != EOF) {
        print("t");
        printf("char = %c\n", c);
        if(iscntrl(c)) break;
        else if(isdigit(c) && c != '0') { //CURRENTLY ONLY READING INTEGERS
            print("l");
            if(dsize >= len) {
                len += 1;
                buf = realloc(buf, len + 1 * sizeof(char));
                if(buf == NULL) {
                    perror("function: read_int_file()");
                    return 0;
                }
            }
            print("...");
            buf[dsize++] = (char) c;
            print("nnn");
            buf[dsize++] = '\0';
            print("ohh");
            continue;
        }
        else if(isspace(c)) {
            coo_sparse_mtx[k].size += 1;
            print("space");
            coo_sparse_mtx[k].mtx = realloc(coo_sparse_mtx[k].mtx, coo_sparse_mtx[k].size * sizeof(int *));
            if(coo_sparse_mtx[k].mtx == NULL) {
                perror("function: read_int_file()");
                return 0;
            }
            val = atoi(buf);
            printf("value: %i\n", val);
            if(!add_int_coo(k, val, rows, cols, coo_sparse_mtx[k].size - 1)) {
                perror(NULL);
                return 0;
            }
            cols++;
            if(cols >= coo_sparse_mtx[k].col) { //next row
                rows++;
                cols = 0;
            }
            dsize = 0;
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
int add_int_coo(int k, int val, int row, int col, int index) {
    print("l");
    coo_sparse_mtx[k].mtx[index] = malloc(3  * sizeof(int));
    if(coo_sparse_mtx[k].mtx == NULL) {
        perror("function: add_int_coo()");
        return 0;
    }
    print("2");
    coo_sparse_mtx[k].mtx[index][0] = row;
    coo_sparse_mtx[k].mtx[index][1] = col;
    coo_sparse_mtx[k].mtx[index][2] = val;
    return 1;
}
