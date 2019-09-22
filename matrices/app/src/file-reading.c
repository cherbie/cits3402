#include "mop.h"

/**
 * Return 1 to indicate success and 0 to indicate failure.
 * @param k is the matrix index of the respective file
 */
int read_to_coo(COO **sparse_mtx, int k, int fid) {
    int buffer_len = MAX_MTX_DIMENSIONS; //max number of digits
    int res = 0;
    char *buffer = calloc(5, sizeof(char)); //int or float
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[fid]) != NULL) {
        buffer = str_clean(buffer);
        if(strcmp(buffer, "int") == 0) (*sparse_mtx)[k].is_int = true;
        else (*sparse_mtx)[k].is_int = false;
    }
    buffer = calloc(buffer_len, sizeof(char));
    if(buffer == NULL) {
        perror(NULL);
        return 0;
    }
    if(fgets(buffer, buffer_len - 1, config.fd[fid]) != NULL) {
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
    if(fgets(buffer, buffer_len - 1, config.fd[fid]) != NULL) {
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
        if(!read_coo_filei(sparse_mtx, k, fid)) return 0;
    }
    else {
        if(!read_coo_filef(sparse_mtx, k, fid)) return 0;
    }

    fclose(config.fd[fid]);

    print_coo(&(*sparse_mtx)[k]);
    return 1;
}

/**
 * Read in files of type float into the COO sparse matrix structure.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int read_coo_filef(COO **coo_mtx, int k, int fid) {
    int rows, cols;
    float val = 0.0;
    rows = 0, cols = 0;
    (*coo_mtx)[k].size = 0; //no elements
    (*coo_mtx)[k].mtxf = malloc(1 * sizeof(float *));

    int num = (*coo_mtx)[k].row * (*coo_mtx)[k].col;
    for(int i = 0; i < num; i++) {
        fscanf(config.fd[fid], "%f", &val);
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
int read_coo_filei(COO **coo_mtx, int k, int fid) {
    int rows, cols, val;
    rows = 0, cols = 0; val = 0;
    (*coo_mtx)[k].size = 0; //no elements
    (*coo_mtx)[k].mtxi = malloc(1 * sizeof(int *));

    int num = (*coo_mtx)[k].row * (*coo_mtx)[k].col;

    for(int i = 0; i < num; i++) {
        fscanf(config.fd[fid], "%i", &val);
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
int add_int_coo(COO *coo_mtx, int val, int row, int col, long int index) {
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
int add_float_coo(COO *coo_mtx, float val, int row, int col, long int index) {
    (*coo_mtx).mtxf[index] = malloc(3  * sizeof(float));
    if((*coo_mtx).mtxf == NULL) {
        perror("function: add_int_coo()");
        return 0;
    }
    (*coo_mtx).mtxf[index][0] = (float) row;
    (*coo_mtx).mtxf[index][1] = (float) col;
    (*coo_mtx).mtxf[index][2] = (float) val;
    return 1;
}

// -- Coordinate Sparse Row Matrix Representation --

/*
 * Read non-zero values from file into CSR struct.
 * @return 1 to indicate success and zero to indicate failure.
 */
int read_to_csr(CSR **sparse_mtx, int k, int fid) {
    COO *coo_mtx = malloc(1 * sizeof(COO));
    if(!read_to_coo(&coo_mtx, k, fid)) {
        fprintf(stderr, "Error reading into sparse matrix.\n");
        return 0;
    }

    // -- CONVERT COO TO CSR --

    if(!coo2csr(&coo_mtx[k], &(*sparse_mtx)[k])) {
        fprintf(stderr, "Error reading into sparse matrix.\n");
        return 0;
    }
    dealloc_coo(&coo_mtx, 1);
    free(coo_mtx);
    return 1;
}

/**
 * Read matrix values given in a COO structure.
 * Convert to CSR struct
 * @param csr_mtx CSR struct *
 */
int coo2csr(COO *coo_mtx, CSR *csr_mtx) {
    (*csr_mtx).is_int = (*coo_mtx).is_int;
    (*csr_mtx).row = (*coo_mtx).row;
    (*csr_mtx).col = (*coo_mtx).col;
    (*csr_mtx).size = (*coo_mtx).size;

    if((*coo_mtx).is_int) { //int
        (*csr_mtx).mtxi = malloc((*csr_mtx).size * sizeof(int));
        (*csr_mtx).mtx_offset = calloc((*csr_mtx).row + 1, sizeof(int));
        (*csr_mtx).mtx_col = malloc(((*csr_mtx).size) * sizeof(int));
        if((*csr_mtx).mtxi == NULL || (*csr_mtx).mtx_col == NULL || (*csr_mtx).mtx_offset == NULL) {
            perror("function: read_coo2csc()");
            return 0;
        }
        int count = 0;
        for(int i = 0; i < (*csr_mtx).row; i++) {
            for(int j = 0; j < (*coo_mtx).size; j++) {
                if((*coo_mtx).mtxi[j][0] == i) {
                    (*csr_mtx).mtx_offset[(*coo_mtx).mtxi[j][0] + 1] += 1;
                    (*csr_mtx).mtx_col[count] = (*coo_mtx).mtxi[j][1];
                    (*csr_mtx).mtxi[count] = (*coo_mtx).mtxi[j][2];
                    count++;
                }
            }
        }
    }
    else {
        (*csr_mtx).mtxf = malloc((*csr_mtx).size * sizeof(float));
        (*csr_mtx).mtx_offset = calloc((*csr_mtx).row + 1, sizeof(int));
        (*csr_mtx).mtx_col = malloc(((*csr_mtx).size) * sizeof(int));
        if((*csr_mtx).mtxf == NULL || (*csr_mtx).mtx_col == NULL || (*csr_mtx).mtx_offset == NULL) {
            perror("function: read_coo2csc()");
            return 0;
        }
        int count = 0;
        for(int i = 0; i < (*csr_mtx).row; i++) {
            for(int j = 0; j < (*coo_mtx).size; j++) {
                if((*coo_mtx).mtxf[j][0] == i) {
                    (*csr_mtx).mtx_offset[(int)(*coo_mtx).mtxf[j][0] + 1] += 1;
                    (*csr_mtx).mtx_col[count] = (*coo_mtx).mtxf[j][1];
                    (*csr_mtx).mtxf[count] = (*coo_mtx).mtxf[j][2];
                    count++;
                }
            }
        }
    }
    return 1;
}


// -- READ INTO CSC STRUCT --

int read_to_csc(CSC **sparse_mtx, int k, int fid) {
    COO *coo_mtx = malloc(1 * sizeof(COO));
    if(!read_to_coo(&coo_mtx, k, fid)) {
        fprintf(stderr, "Error reading into sparse matrix.\n");
        return 0;
    }

    // -- CONVERT COO TO CCS --

    if(!coo2csc(&coo_mtx[k], &(*sparse_mtx)[k])) {
        fprintf(stderr, "Error reading into sparse matrix.\n");
        return 0;
    }
    dealloc_coo(&coo_mtx, 1);
    free(coo_mtx);
    return 1;
}

/**
 * Read matrix values given in a COO structure.
 * Conversion to CSC struct
 * @param csc_mtx CSC struct
 */
int coo2csc(COO *coo_mtx, CSC *csc_mtx) {
    (*csc_mtx).is_int = (*coo_mtx).is_int;
    (*csc_mtx).row = (*coo_mtx).row;
    (*csc_mtx).col = (*coo_mtx).col;
    (*csc_mtx).size = (*coo_mtx).size;

    if((*coo_mtx).is_int) { //int
        (*csc_mtx).mtxi = malloc((*csc_mtx).size * sizeof(int));
        (*csc_mtx).mtx_offset = calloc((*csc_mtx).col + 1, sizeof(int));
        (*csc_mtx).mtx_row = malloc(((*csc_mtx).size) * sizeof(int));
        if((*csc_mtx).mtxi == NULL || (*csc_mtx).mtx_row == NULL || (*csc_mtx).mtx_offset == NULL) {
            perror("function: read_coo2csc()");
            return 0;
        }
        int count = 0;
        for(int i = 0; i < (*csc_mtx).col; i++) {
            for(int j = 0; j < (*coo_mtx).size; j++) {
                if((*coo_mtx).mtxi[j][1] == i) {
                    (*csc_mtx).mtx_offset[(*coo_mtx).mtxi[j][1] + 1] += 1;
                    (*csc_mtx).mtx_row[count] = (*coo_mtx).mtxi[j][0];
                    (*csc_mtx).mtxi[count] = (*coo_mtx).mtxi[j][2];
                    count++;
                }
            }
        }
    }
    else {
        (*csc_mtx).mtxf = malloc((*csc_mtx).size * sizeof(float));
        (*csc_mtx).mtx_offset = calloc((*csc_mtx).col + 1, sizeof(int));
        (*csc_mtx).mtx_row = malloc(((*csc_mtx).size) * sizeof(int));
        if((*csc_mtx).mtxf == NULL || (*csc_mtx).mtx_row == NULL || (*csc_mtx).mtx_offset == NULL) {
            perror("function: read_coo2csc()");
            return 0;
        }
        int count = 0;
        for(int i = 0; i < (*csc_mtx).col; i++) {
            for(int j = 0; j < (*coo_mtx).size; j++) {
                if((*coo_mtx).mtxf[j][1] == i) {
                    (*csc_mtx).mtx_offset[(int)(*coo_mtx).mtxf[j][1] + 1] += 1;
                    (*csc_mtx).mtx_row[count] = (*coo_mtx).mtxf[j][0];
                    (*csc_mtx).mtxf[count] = (*coo_mtx).mtxf[j][2];
                    count++;
                }
            }
        }
    }
    return 1;
}
