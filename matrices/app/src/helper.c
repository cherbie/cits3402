#include "mop.h"

void print_usage(void) {
    printf("Usage: mop [-sc|-tr|-ad|-ts|-mm] [-l filename] -f %s [%s]\n", "%s", "%s");
}

void initialise(void) {
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

    //function pointers
    op_func[0] = scalar_mp;
    op_func[1] = trace;
    op_func[2] = addition;
    op_func[3] = transpose_matrix;
    op_func[4] = matrix_mp;
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
    printf("Configuration ::\n");
    printf(" ... Matrix operation -> %s\n", op_to_string());
    printf(" ... Input 1 -> %s\n", config.in1_filename);
    printf(" ... Input 2 -> %s\n", config.in2_filename);
    printf(" ... Logger file -> %s\n", config.log_filename);
}

void print(char *str) {
    printf("%s\n", str);
}
