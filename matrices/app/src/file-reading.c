#include "mop.h"

/**
 * Return 1 to indicate success and 0 to indicate failure.
 * @param k is the matrix index of the respective file
 */
int read_to_coo(COO **sparse_mtx, int k) {
    int buffer_len = MAX_MTX_DIMENSIONS; //max number of digits
    int res = 0;
    char *buffer = calloc(5, sizeof(char)); //int or float
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        buffer = str_clean(buffer);
        if(strcmp(buffer, "int") == 0) (*sparse_mtx)[k].is_int = true;
        else (*sparse_mtx)[k].is_int = false;
    }
    buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        buffer = str_clean(buffer);
        res = sscanf(buffer, "%i", &(*sparse_mtx)[k].row);
        if(res == 0 || res == EOF) {
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
        res = sscanf(buffer, "%i", &(*sparse_mtx)[k].col);
        if(res == 0 || res == EOF) {
            perror(NULL);
            return 0;
        }
    }
    free(buffer);
    // -- READ MATRIX VALUES --
    if((*sparse_mtx)[k].is_int) {
        if(!read_coo_filei(sparse_mtx, k)) return 0;
    }
    else {
        if(!read_coo_filef(sparse_mtx, k)) return 0;
    }

    fclose(config.fd[k]);

    print_coo(&(*sparse_mtx)[k]);
    return 1;
}

/**
 * Read in files of type float into the COO sparse matrix structure.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int read_coo_filef(COO **coo_mtx, int k) {
    int rows, cols;
    float val = 0.0;
    rows = 0, cols = 0;
    (*coo_mtx)[k].size = 0; //no elements
    (*coo_mtx)[k].mtxf = malloc(1 * sizeof(float *));

    int num = (*coo_mtx)[k].row * (*coo_mtx)[k].col;
    for(int i = 0; i < num; i++) {
        fscanf(config.fd[k], "%f", &val);
        fprintf(stdout, "\nFloat: %f\n", val);
        if(val == 0.0) { //value should not be recorded in sparse matrix
            cols++;
            if(cols >= (*coo_mtx)[k].col) { //next row
                rows++;
                cols = 0;
            }
            continue;
        }
        (*coo_mtx)[k].size += 1;
        (*coo_mtx)[k].mtxf = realloc((*coo_mtx)[k].mtxf, (*coo_mtx)[k].size * sizeof(float *));
        if((*coo_mtx)[k].mtxf == NULL) {
            perror("function: read_int_file()");
            return 0;
        }
        fprintf(stdout, "\n float: %f\n", val);
        if(!add_float_coo(&(*coo_mtx)[k], val, rows, cols, (*coo_mtx)[k].size - 1)) {
            perror(NULL);
            return 0;
        }
        cols++;
        if(cols >= (*coo_mtx)[k].col) { //next row
            rows++;
            cols = 0;
        }
    }
    return 1;
}

/**
 * Reads integer values into COO sparse matrix structure.
 * @param k is the matrix index of the file underquestion.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int read_coo_filei(COO **coo_mtx, int k) {
    int rows, cols, val;
    rows = 0, cols = 0; val = 0;
    (*coo_mtx)[k].size = 0; //no elements
    (*coo_mtx)[k].mtxi = malloc(1 * sizeof(int *));

    int num = (*coo_mtx)[k].row * (*coo_mtx)[k].col;

    for(int i = 0; i < num; i++) {
        fscanf(config.fd[k], "%i", &val);
        fprintf(stdout, "\ninteger: %i\n", val);
        if(val == 0) { //value should not be recorded in sparse matrix
            cols++;
            if(cols >= (*coo_mtx)[k].col) { //next row
                rows++;
                cols = 0;
            }
            continue;
        }
        (*coo_mtx)[k].size += 1;
        (*coo_mtx)[k].mtxi = realloc((*coo_mtx)[k].mtxi, (*coo_mtx)[k].size * sizeof(int *));
        if((*coo_mtx)[k].mtxi == NULL) {
            perror("function: read_int_file()");
            return 0;
        }
        fprintf(stdout, "\n int: %i\n", val);
        if(!add_int_coo(&(*coo_mtx)[k], val, rows, cols, (*coo_mtx)[k].size - 1)) {
            perror(NULL);
            return 0;
        }
        cols++;
        if(cols >= (*coo_mtx)[k].col) { //next row
            rows++;
            cols = 0;
        }
    }
    return 1;
}

/**
 * Dynamically creates Sparse Coordinate matrix of input file.
 * @param k is the application file identifier.
 * @count is the array index of the coordinates.
 */
int add_int_coo(COO *coo_mtx, int val, int row, int col, int index) {
    (*coo_mtx).mtxi[index] = malloc(3  * sizeof(int));
    if((*coo_mtx).mtxi == NULL) {
        perror("function: add_int_coo()");
        return 0;
    }
    (*coo_mtx).mtxi[index][0] = row;
    (*coo_mtx).mtxi[index][1] = col;
    (*coo_mtx).mtxi[index][2] = val;
    return 1;
}

/**
 * Dynamically creates Sparse Coordinate matrix of input file.
 * @param k is the application file identifier.
 * @count is the array index of the coordinates.
 */
int add_float_coo(COO *coo_mtx, float val, int row, int col, int index) {
    (*coo_mtx).mtxf[index] = malloc(3  * sizeof(float));
    if((*coo_mtx).mtxf == NULL) {
        perror("function: add_int_coo()");
        return 0;
    }
    (*coo_mtx).mtxf[index][0] = (float) row;
    (*coo_mtx).mtxf[index][1] = (float) col;
    (*coo_mtx).mtxf[index][2] = val;
    return 1;
}

// -- Coordinate Sparse Row Matrix Representation --

int read_to_csr(CSR **sparse_mtx, int k) {
    int buffer_len = MAX_MTX_DIMENSIONS; //max number of digits
    int res = 0;
    char *buffer = calloc(5, sizeof(char)); //int or float
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        buffer = str_clean(buffer);
        if(strcmp(buffer, "int") == 0) (*sparse_mtx)[k].is_int = true;
        else (*sparse_mtx)[k].is_int = false;
    }
    buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[k]) != NULL) {
        buffer = str_clean(buffer);
        res = sscanf(buffer, "%i", &(*sparse_mtx)[k].row);
        if(res == 0 || res == EOF) {
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
        res = sscanf(buffer, "%i", &(*sparse_mtx)[k].col);
        if(res == 0 || res == EOF) {
            perror(NULL);
            return 0;
        }
    }
    // -- READ MATRIX VALUES --
    if((*sparse_mtx)[k].is_int) {
        if(!read_csr_filei(sparse_mtx, k)) return 0;
    }
    else {
        if(!read_csr_filef(sparse_mtx, k)) return 0;
    }
    return 1;
}

/**
 * Read matrix values given in a file.
 * @param csr_mtx CSR struct
 */
int read_csr_filei(CSR **csr_mtx, int k) {
    int val = 0;
    int x,y;
    x = 0; y = 0; //immediate row and column respectively
    (*csr_mtx)[k].size = 0; //no elements
    (*csr_mtx)[k].mtxi = malloc(1 * sizeof(int));
    (*csr_mtx)[k].mtx_col = malloc(1 * sizeof(int));
    (*csr_mtx)[k].mtx_offset = calloc((*csr_mtx)[k].row, sizeof(int)); //offset size defined
    int num = (*csr_mtx)[k].row * (*csr_mtx)[k].col; //number of matrix elements
    for(int i = 0; i < num; i++) {
        fscanf(config.fd[k], "%i", &val);
        fprintf(stdout, "\ninteger: %i\n", val);
        if(val == 0) { //value should not be recorded in sparse matrix
            continue;
        }
        x = i / (*csr_mtx)[k].row; //calculate row
        y = i % (*csr_mtx)[k].col; //calculate column
        (*csr_mtx)[k].size += 1; //increment number of non-zero values
        (*csr_mtx)[k].mtxi = realloc((*csr_mtx)[k].mtxi, (*csr_mtx)[k].size * sizeof(int));
        (*csr_mtx)[k].mtx_col = realloc((*csr_mtx)[k].mtx_col, (*csr_mtx)[k].size * sizeof(int));
        if((*csr_mtx)[k].mtxi == NULL || (*csr_mtx)[k].mtx_col == NULL) {
            perror("function: read_int_file()");
            return 0;
        }
        fprintf(stdout, "\n int: %i\n", val);
        if(!add_int_csr(&(*csr_mtx)[k], val, x, y, (*csr_mtx)[k].size - 1)) {
            perror(NULL);
            return 0;
        }
    }
    return 1;
}

/**
 * Read matrix values given in a file.
 * @param csr_mtx CSR struct
 */
int read_csr_filef(CSR **csr_mtx, int k) {
    float val = 0;
    int x,y;
    x = 0; y = 0; //immediate row and column respectively
    (*csr_mtx)[k].size = 0; //no elements
    (*csr_mtx)[k].mtxf = malloc(1 * sizeof(float));
    (*csr_mtx)[k].mtx_col = malloc(1 * sizeof(int));
    (*csr_mtx)[k].mtx_offset = calloc((*csr_mtx)[k].row, sizeof(int)); //offset size defined
    int num = (*csr_mtx)[k].row * (*csr_mtx)[k].col; //number of matrix elements
    for(int i = 0; i < num; i++) {
        fscanf(config.fd[k], "%f", &val);
        fprintf(stdout, "\nFLOAT: %f\n", val);
        if(val == 0.0) { //value should not be recorded in sparse matrix
            continue;
        }
        x = i / (*csr_mtx)[k].row; //calculate row
        y = i % (*csr_mtx)[k].col; //calculate column
        (*csr_mtx)[k].size += 1; //increment number of non-zero values
        (*csr_mtx)[k].mtxf = realloc((*csr_mtx)[k].mtxf, (*csr_mtx)[k].size * sizeof(float));
        (*csr_mtx)[k].mtx_col = realloc((*csr_mtx)[k].mtx_col, (*csr_mtx)[k].size * sizeof(int));
        if((*csr_mtx)[k].mtxf == NULL || (*csr_mtx)[k].mtx_col == NULL) {
            perror("function: read_int_file()");
            return 0;
        }
        fprintf(stdout, "\n float: %f\n", val);
        if(!add_float_csr(&(*csr_mtx)[k], val, x, y, (*csr_mtx)[k].size - 1)) {
            perror(NULL);
            return 0;
        }
    }
    return 1;
}

/**
 * Update structure values
 * @param count is the index of the non-zero value.
 */
int add_int_csr(CSR *csr_mtx, int val, int row, int col, int count) {
    (*csr_mtx).mtxi[count] = val;
    (*csr_mtx).mtx_offset[row] += 1;
    (*csr_mtx).mtx_col[count] = col;
    return 1;
}

int add_float_csr(CSR *csr_mtx, float val, int row, int col, int count) {
    (*csr_mtx).mtxf[count] = val;
    (*csr_mtx).mtx_offset[row] += 1;
    (*csr_mtx).mtx_col[count] = col;
    return 1;
}
