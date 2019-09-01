#include "mop.h"

int main(int argc, char *argv[]) {
    initialise();
    if (argc == 1) {
        print_usage();
        exit(EXIT_FAILURE);
    } else if(!parse_cmd(argc, argv)) {
        fprintf(stderr, "Error parsing command line arguments\n");
        exit(EXIT_FAILURE);
    } else {
        printf("... Program complete.\n");
        exit(EXIT_SUCCESS);
    }
}

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
}

char *op_to_string(void) {
    switch(*config.operation) {
        case 0: op_str = strdup("sc"); break;
        case 1: op_str = strdup("tr"); break;
        case 2: op_str = strdup("ad"); break;
        case 3: op_str = strdup("ts"); break;
        case 4: op_str = strdup("mm"); break;
        default: return NULL;
    }
    return op_str;
}
