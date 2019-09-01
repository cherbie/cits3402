#include "mop.h"

/**
 * Process the supplied command line arguments and manage the following execution steps.
 * @return 1 to indicate success and 0 to indicate error
 */
int parse_cmd(char *argv[]) {
    int op = get_operation(argv[1]);
    if(op < 0) {
        errno = ENOEXEC;
        perror("Invalided command line arguments provided.\n");
        return 0;
    }
    switch(op ) {
        case 0: printf("-sc\n"); break;
        case 1: printf("-tr\n"); break;
        case 2: printf("-ad\n"); break;
        case 3: printf("-ts\n"); break;
        case 4: printf("-mm\n"); break;
        default: {
            errno = ENOEXEC;
            perror("Error parsing command line arguments.\nMatrix operation needs to be specified.\n");
            return 0;
        }
    }
    return 1;
}

int get_operation(char *arg) {
    for(int i = 0; i < NUM_OPERATIONS; i++) {
        if(strcmp(arg, arg_operations[i]) == 0)
            return i;
    }
    return -1;
}