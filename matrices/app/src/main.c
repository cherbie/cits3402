#include "mop.h"

int main(int argc, char *argv[]) {
    if(!initialise()) {
        fprintf(stderr, "Error initialising global variables\n");
        exit(EXIT_FAILURE);
    }
    if(argc == 1) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    if(!parse_cmd(argc, argv)) {
        fprintf(stderr, "Error parsing command line arguments.\n");
        exit(EXIT_FAILURE);
    }
    if(!operation_main()) exit(EXIT_FAILURE);
    fprintf(stdout, "%s\n%s\n", config.op_str, config.filename[0]);
    fprintf(stdout, "%6.4f\n%6.4f\n", config.time[0].delta, config.time[1].delta);
    //print_config();
    dealloc_config();
    //printf(" ... Program complete.\n");
    exit(EXIT_SUCCESS);
}
