#include "mop.h"

int main(int argc, char *argv[]) {
    initialise();
    if (argc == 1) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    if(!parse_cmd(argc, argv)) {
        fprintf(stderr, "Error parsing command line arguments\n");
        exit(EXIT_FAILURE);
    }
    print_config();
    exit(0);
    if(!operation_main()) {
        fprintf(stderr, "Unable to perform matrix operation.\n");
        exit(EXIT_FAILURE);
    }
    printf(" ... Program complete.\n");
    exit(EXIT_SUCCESS);
}
