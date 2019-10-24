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
                //printf("%s | %i\n", (*config).filename_in, optind);
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
 * Logs the execution time analysis to file.
 * @return 0 to indicate success and -1 to indicate failure.
 */
int log_time(SP_CONFIG *config, PATHS *paths) {
    time_t rawtime;
    struct tm *exec_time;

    time(&rawtime);
    exec_time = localtime(&rawtime);

    char *buffer = calloc(50, sizeof(char));
    char *date = malloc(20 * sizeof(char));
    char *info = malloc(20 * sizeof(char));
    if(buffer == NULL || date == NULL || info == NULL) {
        perror(NULL);
        return -1;
    }

    sprintf(date, "%02.0f%02.0f%i", (float) exec_time->tm_mday, (float) exec_time->tm_mon + 1.0, exec_time->tm_year + 1900);
    sprintf(info, "%d_%d", (*config).nproc, (*paths).nodes);
    sprintf(buffer, "./time/%s_%s_%s.out", "time", date, info);

    (*config).time_out = calloc(50, sizeof(char));
    if((*config).time_out == NULL) {
        perror(NULL);
        return -1;
    }
    (*config).time_out = memcpy(&(*config).time_out[0], &buffer[0], sizeof(char) * (strlen(buffer) + 1));

    (*config).fp_time_out = fopen((*config).time_out, "w"); //writing in binary mode
    if((*config).fp_time_out == NULL) {
        perror("location: /parser.c/set_logger()\n");
        return -1;
    }
    free(buffer);
    free(date);
    free(info);

    double delta = (*config).endtime - (*config).starttime;
    // -- WRITE --
    fprintf((*config).fp_time_out, "Processes:\t%i\n", (*config).nproc);
    fprintf((*config).fp_time_out, "Rows:\t%i\n", (*paths).nodes);
    fprintf((*config).fp_time_out, "Execution time:\t%.12f\n", delta);

    fclose((*config).fp_time_out);
    free((*config).time_out);
    return 0;
}
