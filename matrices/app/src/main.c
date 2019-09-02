#include "mop.h"

int main(int argc, char *argv[]) {
    initialise();
    if (argc == 1) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    if(!parse_cmd(argc, (const char**) argv)) {
        fprintf(stderr, "Error parsing command line arguments\n");
        exit(EXIT_FAILURE);
    }
    print_config();
    if(!operation_main()) {
        fprintf(stderr, "Unable to perform matrix operation.\n");
        exit(EXIT_FAILURE);
    }
    printf(" ... Program complete.\n");
    exit(EXIT_SUCCESS);
}
