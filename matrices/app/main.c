#include "mop.h"

int main(int argc, char *argv[]) {
    initialise();
    if (argc == 1) {
        print_usage();
        exit(EXIT_FAILURE);
    } else if(!parse_cmd(argv)) {
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
    strcpy(arg_operations[0], "-sc");
    strcpy(arg_operations[1], "-tr");
    strcpy(arg_operations[2], "-ad");
    strcpy(arg_operations[3], "-ts");
    strcpy(arg_operations[4], "-mm");
}