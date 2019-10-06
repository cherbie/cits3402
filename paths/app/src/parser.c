#include "sp.h"

int parse_args(SP_CONFIG *config, int *argc, char ***argv)  {
    int c = 0;
    while(true) {
        c = getopt((*argc), *argv, "+:f:");
        if (c == -1) break; // no more elements
        switch (c) {
            case 'f': { // file specified
                if(optind > *argc) return -1;
                (*config).filename_in = strdup(optarg);
                if((*config).filename_in == NULL) return -1;
                printf("%s | %i\n", (*config).filename_in, optind);
                return 0;
            }
            case ':': {
                if((*config).rank == ROOT) fprintf(stderr, "Error: Missing option argument.\n");
                return -1;
            }
            case '?': {
                if((*config).rank == ROOT) fprintf(stderr, "Error: Unrecognised argument provided.\n");
                return -1;
            }
            default: {
                if((*config).rank == ROOT) fprintf(stderr, "Error: Please specify option arguments.\n");
                return -1;
            }
        }
    }
    return 0;
}
