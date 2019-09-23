#include "mop.h"

/**
 * Set the config / operation of execution.
 * return 1 to indicate success and 0 to indicate failure
 */
int parse_cmd(int argc, char *argv[]) {
    int c = 0;
    struct option long_options[NUM_OPERATIONS + 1] = {
        {"sc",  required_argument, NULL, '0'},
        {"tr",  no_argument, NULL, '1'},
        {"ad",  no_argument, NULL, '2'},
        {"ts",  no_argument, NULL, '3'},
        {"mm",  no_argument, NULL, '4'},
        {0,0,0,0}
    };
    bool operation_seen = false;
    while (1) {
        c = getopt_long(argc, argv, "-:f:lst:", long_options, &config.operation);
        if (c == -1) break;
        switch (c) {
            case 1: {
                if((optind >= argc) & !set_input_files(2, argv[optind -1])) {
                    return 0;
                }
                break;
            }
            case '0': {
                if(operation_seen) {
                    fprintf(stderr, "More than one matrix operation specified.\n");
                    return 0;
                }
                if((optind >= argc) & !set_config_sm(optarg)) return 0;
                operation_seen = true;
                break;
            }
            case '1': case '2': case '3': case '4': {
                if(operation_seen) {
                    fprintf(stderr, "More than one matrix operation specified.\n");
                    return 0;
                }
                operation_seen = true;
                break;
            }
            case 'f': { //file
                if((optind >= argc) & !set_input_files(1, optarg)) {
                    return 0;
                }
                break;
            }
            case 't': {
                if((optind >= argc) & !set_config_threads(optarg)) return 0;
                break;
            }
            case 'l': { //log
                if(!set_logger()) return 0;
                break;
            }
            case 's' : {
                config.sync = true;
                break;
            }
            case ':': {
                fprintf(stderr, "Missing option argument.\n");
                return 0;
            }
            default: break;
        }
    }
    if(config.num_threads < 0) config.num_threads = DEFAULT_THREAD_COUNT;
    if(!config_is_setup()) return 0;
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure
 */
bool config_is_setup(void) {
    if(config.operation < 0) {
        fprintf(stderr, "Error: Incorrect or no matrix operation option specified.\n");
        return false;
    }else if(config.fd[0] == NULL) {
        fprintf(stderr, "Error: No input files specified.\n");
        return false;
    }else return true;
}


/**
 *
 * Return 1 to indicate success and 0 to indicate failure.
 */
int set_input_files(int file_id, char *name) {
    if((config.fd[file_id - 1] = fopen(name, "r")) == NULL) {
        perror("location: /parser.c/set_input_files()\n");
        fprintf(stderr, "Error reading files.\n");
        return 0;
    }
    if((config.filename[file_id - 1] = strdup(name)) == NULL) {
        perror("location: /parser.c/set_input_files()\n");
        fprintf(stderr, "Error reading files.\n");
        return 0;
    }
    return 1;
}

int set_config_threads(char * arg) {
    int n = atoi(arg);
    if(n <= 0) {
        fprintf(stderr, "Number of threads specified needs to be a positive integer.\n");
        return 0;
    }
    config.num_threads = n;
    return 1;
}

int set_config_sm(char *arg) {
    //atof;
    float f = atof(arg);
    if(f == 0) {
        fprintf(stderr, "Either an error or scalar of zero was supplied.\n");
        return 0;
    }
    config.sm = f;
    return 1;
}
/**
 * Return 1 to indicate success and 0 to indicate failure.
 * @parama flag 1 indicates filename is not defined
 */
int set_logger(void) {
    char *buffer = calloc(30, sizeof(char));
    char *date = malloc(11 * sizeof(char));
    char *time = malloc(5 * sizeof(char));
    strncpy(buffer, STUDENT_NUMBER, strlen(STUDENT_NUMBER));
    sprintf(date, "%02.0f%02.0f%i", (float) config.exec_time->tm_mday, (float) config.exec_time->tm_mon + 1.0, config.exec_time->tm_year + 1900);
    sprintf(time, "%02.0f%02.0f", (float) config.exec_time->tm_hour, (float) config.exec_time->tm_min);
    sprintf(buffer, "%s_%s_%s_%s.out", STUDENT_NUMBER, date, time, op_to_string());
    free(date); free(time);
    config.log_filename = strdup(buffer);
    if(config.log_filename == NULL) {
        perror("location: /parser.c/set_logger()\n");
        return 0;
    }
    config.log_fd = fopen(config.log_filename, "w");
    if(config.log_fd == NULL) {
        perror("location: /parser.c/set_logger()\n");
        return 0;
    }
    free(buffer);
    return 1;
}
