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


/**
 * Return 0 to indicate success and -1 to indicate failure.
 */
int set_logger(SP_CONFIG *config) {
    time_t rawtime;
    struct tm *exec_time;
    time(&rawtime);
    exec_time = localtime(&rawtime);
    char *buffer = calloc(30, sizeof(char));
    char *date = malloc(11 * sizeof(char));
    char *time = malloc(5 * sizeof(char));
    if(buffer == NULL || date == NULL || time == NULL) {
        perror(NULL);
        return -1;
    }
    sprintf(date, "%02.0f%02.0f%i", (float) exec_time->tm_mday, (float) exec_time->tm_mon + 1.0, exec_time->tm_year + 1900);
    sprintf(time, "%02.0f%02.0f", (float) exec_time->tm_hour, (float) exec_time->tm_min);
    sprintf(buffer, "%d_%s_%s.out", STUDENT_NUMBER, date, time);
    (*config).filename_out = strdup(buffer);
    if((*config).filename_out == NULL) {
        perror("location: /parser.c\n");
        return -1;
    }
    (*config).fp_out = fopen((*config).filename_out, "wb"); //writing in binary mode
    if((*config).fp_out == NULL) {
        perror("location: /parser.c/set_logger()\n");
        return -1;
    }
    free(buffer);
    free(date);
    free(time);
    return 0;
}

/**
 * Log the final all pairs
 * @return 0 to indicate success and -1 to indicate failure.
 */
int log_result(SP_CONFIG *config, PATHS *paths) {
    int nmemb, rout;
    // -- WRITE SIZE OF ROW --
    nmemb = 1;
    rout = fwrite(&(*paths).nodes, sizeof(int), nmemb, (*config).fp_out);
    if(rout != nmemb) {
        perror(NULL);
        return -1;
    }

    // -- WRITE APSP --
    nmemb = (*paths).nodes * (*paths).nodes;
    rout = fwrite(&(*paths).sp[0], sizeof(int), nmemb, (*config).fp_out);
    if(rout != nmemb) {
        perror(NULL);
        return -1;
    }
    return 0;
}
