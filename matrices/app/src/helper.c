#include "mop.h"

void print_usage(void) {
    printf("Usage: mop [-sc|-tr|-ad|-ts|-mm] [-l filename] -f %s [%s]\n", "%s", "%s");
}

int initialise(void) {
    time(&rawtime);
    exec_time = localtime(&rawtime);
    arg_options[0] = strdup("-sc");
    arg_options[1] = strdup("-tr");
    arg_options[2] = strdup("-ad");
    arg_options[3] = strdup("-ts");
    arg_options[4] = strdup("-mm");
    arg_options[5] = strdup("-l");
    arg_options[6] = strdup("-f");

    config.operation = -1;
    config.num_threads = -1;
    config.fd = malloc(NUMBER_OF_INPUT_FILES * sizeof(int *));
    config.filename = malloc(NUMBER_OF_INPUT_FILES * sizeof(char *));
    if(config.fd == NULL || config.filename == NULL) {
        perror(NULL);
        return 0;
    }
    config.log_fd = NULL;
    config.log_filename = NULL;

    //function pointers
    op_func[0] = scalar_mp;
    op_func[1] = trace;
    op_func[2] = addition;
    op_func[3] = transpose_matrix;
    op_func[4] = matrix_mp;

    //SPARSE MATRIX REP.
    if((coo_sparse_mtx = malloc(NUMBER_OF_INPUT_FILES * sizeof(COO))) == NULL) {
        perror(NULL);
        return 0;
    }
    return 1;
}

char *op_to_string(void) {
    switch(config.operation) {
        case 0: op_str = strdup("sc"); break;
        case 1: op_str = strdup("tr"); break;
        case 2: op_str = strdup("ad"); break;
        case 3: op_str = strdup("ts"); break;
        case 4: op_str = strdup("mm"); break;
        default: return NULL;
    }
    return op_str;
}

void print_config() {
    printf(" -- Configuration --\n");
    printf(" ... Matrix operation -> %s\n", op_to_string());
    printf(" ... Input 1 -> %s\n", config.filename[0]);
    printf(" ... Input 2 -> %s\n", config.filename[1]);
    printf(" ... Logger file -> %s\n", config.log_filename);
    if(config.num_threads > 0) printf(" ... Number of threads -> %i\n", config.num_threads);
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

void print_coo(int k) {
    print(" --- ");
    for(int i = 0; i < coo_sparse_mtx[k].size; i++) {
        printf(" %i ", coo_sparse_mtx[k].mtx[i][0]);
        printf(" %i ", coo_sparse_mtx[k].mtx[i][1]);
        printf(" %i ", coo_sparse_mtx[k].mtx[i][2]);
        print("");
    }
    print(" ---- ");
}
