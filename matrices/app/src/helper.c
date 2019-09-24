#include "mop.h"

void print_usage(void) {
    printf("Usage: mop [-sc|-tr|-ad|-ts|-mm] [-l filename] -f %s [%s]\n", "%s", "%s");
}

int initialise(void) {
    time(&config.rawtime);
    config.exec_time = localtime(&config.rawtime);

    arg_options[0] = strdup("-sc");
    arg_options[1] = strdup("-tr");
    arg_options[2] = strdup("-ad");
    arg_options[3] = strdup("-ts");
    arg_options[4] = strdup("-mm");
    arg_options[5] = strdup("-l");
    arg_options[6] = strdup("-f");

    config.operation = -1;
    config.num_threads = -1;
    config.num_files = -1;
    config.sync = false;
    config.fd = malloc(NUMBER_OF_INPUT_FILES * sizeof(FILE *));
    config.filename = malloc(NUMBER_OF_INPUT_FILES * sizeof(char *));
    if(config.fd == NULL || config.filename == NULL) {
        perror(NULL);
        return 0;
    }
    config.log_fd = NULL;
    config.log_filename = NULL;
    config.time = malloc(2 * sizeof(TIME_STAT));

    //function pointers
    op_func[0] = scalar;
    op_func[1] = trace;
    //op_func[2] = addition;
    op_func[3] = transpose_matrix;
    op_func[4] = matrix_mp;

    return 1;
}

char *op_to_string(void) {
    switch(config.operation) {
        case 0: config.op_str = strdup("sc"); break;
        case 1: config.op_str = strdup("tr"); break;
        case 2: config.op_str = strdup("ad"); break;
        case 3: config.op_str = strdup("ts"); break;
        case 4: config.op_str = strdup("mm"); break;
        default: return NULL;
    }
    return config.op_str;
}

void print_config() {
    printf(" -- Configuration --\n");
    printf(" ... Matrix operation -> %s\n", op_to_string());
    printf(" ... Input 1 -> %s\n", config.filename[0]);
    printf(" ... Logger file -> %s\n", config.log_filename);
    if(config.num_threads > 0) printf(" ... Number of threads -> %i\n", config.num_threads);
    printf(" ... file reading time -> %12.10f\n", (float) config.time[0].delta);
    printf(" ... operation processing time - > %12.10f\n", (float) config.time[1].delta);
    printf("\n");
}

void print(char *str) {
    printf("%s\n", str);
}

char *str_clean(char *str) {
    int i,j = 0;
    for(i = 0; str[i] != '\0'; i++) {
        while(!((str[i] >= 'a' && str[i]<='z') || (str[i]>='A' && str[i]<='Z') || (str[i]>='0' && str[i]<='9') || str[i]=='\0')){
            for(j = i; str[j] != '\0'; ++j) {
                str[j] = str[j+1];
            }
            str[j] = '\0';
        }
    }
    return str;
}

void print_coo(COO *coo_mtx) {
    print(" --- ");
    if((*coo_mtx).is_int) {
        for(int i = 0; i < (*coo_mtx).size; i++) {
            printf(" %i ", (*coo_mtx).rowcol[i][0]);
            printf(" %i ", (*coo_mtx).rowcol[i][1]);
            printf(" %i ", (*coo_mtx).mtxi[i]);
            print("\n");
        }
    }
    else {
        for(int i = 0; i < (*coo_mtx).size; i++) {
            printf(" %i ", (*coo_mtx).rowcol[i][0]);
            printf(" %i ", (*coo_mtx).rowcol[i][1]);
            printf(" %3.2f ", (*coo_mtx).mtxf[i]);
            print("\n");
        }
    }
    print(" --- ");
}

void print_csr(CSR *csr_mtx) {
    print(" --- ");
    printf("rows = %i\n", (*csr_mtx).row);
    printf("cols = %i\n", (*csr_mtx).col);
    printf("size = %i\n", (*csr_mtx).size);
    if((*csr_mtx).is_int) {
        for(int i = 0; i < (*csr_mtx).size; i++) {
            printf("%i", (int)(*csr_mtx).mtxi[i]);
        }
        printf("\n");
        for(int i = 0; i <= (*csr_mtx).row; i++) {
            printf("%i", (int)(*csr_mtx).mtx_offset[i]);
        }
        printf("\n");
        for(int i = 0; i < (*csr_mtx).size; i++) {
            printf("%i", (int)(*csr_mtx).mtx_col[i]);
        }
        printf("\n");
    }
    else {
        for(int i = 0; i < (*csr_mtx).size; i++) {
            printf("%3.2f", (*csr_mtx).mtxf[i]);
        }
        printf("\n");
        for(int i = 0; i <= (*csr_mtx).row; i++) {
            printf("%i", (int)(*csr_mtx).mtx_offset[i]);
        }
        printf("\n");
        for(int i = 0; i < (*csr_mtx).size; i++) {
            printf("%i", (int)(*csr_mtx).mtx_col[i]);
        }
        printf("\n");
    }
    print(" --- ");
}

/**
 * Debugging function.
 */
void print_csc(CSC *csc_mtx) {
    print(" --- ");
    printf("rows = %i\n", (*csc_mtx).row);
    printf("cols = %i\n", (*csc_mtx).col);
    printf("size = %i\n", (*csc_mtx).size);
    if((*csc_mtx).is_int) {
        for(int i = 0; i < (*csc_mtx).size; i++) {
            printf("%i", (*csc_mtx).mtxi[i]);
        }
        printf("\n");
        for(int i = 0; i <= (*csc_mtx).col; i++) {
            printf("%i", (*csc_mtx).mtx_offset[i]);
        }
        printf("\n");
        for(int i = 0; i < (*csc_mtx).size; i++) {
            printf("%i", (*csc_mtx).mtx_row[i]);
        }
        printf("\n");
    }
    else {
        for(int i = 0; i < (*csc_mtx).size; i++) {
            printf("%3.2f", (*csc_mtx).mtxf[i]);
            print("\n");
        }
    }
    print(" --- ");
}

/**
 * Calculates whether the COO struct has a non-zero value defined for a particular row & column in a block
 * @returns true if a non-zero value is defined, false otherwise.

bool is_defined(COO *coo_mtx, int row, int col, long int block) {
    if(block >= (*coo_mtx).size) return false;
    else if((*coo_mtx).is_int) {
        if(((*coo_mtx).mtxi[block][0] == row) && (*coo_mtx).mtxi[block][1] == col) return true;
        else return false;
    }
    else {
        if(((*coo_mtx).mtxf[block][0] == (float) row) && ((*coo_mtx).mtxf[block][1] == (float) col)) return true;
        else return false;
    }
}
*/
/**
 * O(n) lookup.
 * @param *csc_mtx CSC struct *
 * @param *seen boolean is nz seen.
 * @param col int for the column underquestion.
 * @returns the index of the non-zero element or -1 if non-exists.
 */
int get_non_zero_csc(CSC *csc_mtx, bool **seen, int col, int row) {
    int start_index = 0;
    int index = 0;
    for(int i = 1; i <= col; i++) start_index += (*csc_mtx).mtx_offset[i-1];
    for(int i = 1; i <= (*csc_mtx).mtx_offset[col]; i++) {
        index = start_index + (i-1);
        if((*csc_mtx).mtx_row[index] != row) continue; //row index of nz value does not equal row under question
        if(!(*seen)[index]) {
            (*seen)[index] = true;
            return index;
        }
    }
    return -1;
}

/**
 * Calculate the difference between start and end time's
 * @return 1 to indicate success and 0 to indicate failure.
 */
int process_stat(void) {
    for(int i = 0; i < 2; i++) {
        config.time[i].delta = (float)((config.time[i].end.tv_sec  -  config.time[i].start.tv_sec) * 1000000u +
           config.time[i].end.tv_usec - config.time[i].start.tv_usec) / 1.e6;
    }
    return 1;
}
