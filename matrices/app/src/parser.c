#include "mop.h"

/**
 * Set the config / operation of execution.
 * return 1 to indicate success and 0 to indicate failure
 */
int parse_cmd(int argc, char *argv[]) {
    int c, digit_optind, this_option_optind = 0;
    struct option long_options[NUM_OPERATIONS + 1] = {
        {"sc",  no_argument, NULL, '0'},
        {"tr",  no_argument, NULL, '1'},
        {"ad",  no_argument, NULL, '2'},
        {"ts",  no_argument, NULL, '3'},
        {"mm",  no_argument, NULL, '4'},
        {0,0,0,0}
    };

    while (1) {
        this_option_optind = optind ? optind : 1;
        printf("this_option_optind %i vs %i\n", this_option_optind, optind);

        c = getopt_long(argc, argv, "-:f:l::", long_options, &config.operation);
        if (c == -1) break;

        printf("char : %c\n", c);
        switch (c) {
            case 1: {
                printf("Non-option argument encountered.\n ... %s\n", optarg);
                if((optind >= argc) & !set_input_files(2, argv[optind -1])) {
                    return 0;
                }
                break;
            }
            case '0': case '1': case '2': case '3': case '4': {
                printf("option %s", long_options[config.operation].name);
                if (optarg) printf(" with arg %s", optarg);
                printf("\n");
                if (digit_optind != 0 && digit_optind != this_option_optind)
                printf("digits occur in two different argv-elements.\n");
                digit_optind = this_option_optind;
                printf("option %i\n", c);
                break;
            }
            case 'f': { //file
                printf("option = f\n");
                if((optind >= argc) & !set_input_files(1, optarg)) {
                    return 0;
                }
                break;
            }
            case 'l': { //log
                printf("option = l\n");
                char *fn = strdup("file");
                if(optind >= argc && optarg == NULL) {
                    if(!set_logger(fn, 1)) {
                        return 0;
                    }
                }else if(!set_logger(optarg, 0)) {
                    return 0;
                }
                break;
            }
            case ':': {
                fprintf(stderr, "Missing option argument following.\n");
                return 0;
            }
            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }
    /* Get all of the non-option arguments
    if (optind < argc) {
        printf("Non-option args: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
    */
    if(config.log_fd == NULL) {
        if(!set_logger(NULL, 1)) {
            return 0;
        }
    }
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
    }else if(config.in1_fd == NULL) {
        fprintf(stderr, "Error: No input files specified.\n");
        return false;
    }else if(config.log_fd == NULL) {
        fprintf(stderr, "Error: Unable to establish output logging file.\n");
        return false;
    }
    else return true;
}


/**
 *
 * Return 1 to indicate success and 0 to indicate failure.
 */
int set_input_files(int file_id, char *name) {
    switch(file_id) {
        case 1: {
            if((config.in1_fd = fopen(name, "r")) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            if((config.in1_filename = strdup(name)) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            return 1;
        }
        case 2: {
            if((config.in2_fd = fopen(name, "r")) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            if((config.in2_filename = strdup(name)) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            return 1;
        }
        default: fprintf(stderr, "Error reading files.\n"); return 0;
    }
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int set_logger(char *filename, int flag) {
    if(flag) {
        char *buffer = malloc((strlen(STUDENT_NUMBER)+1) * sizeof(char));
        char *date = malloc(11 * sizeof(char));
        char *time = malloc(5 * sizeof(char));
        strncpy(buffer, STUDENT_NUMBER, strlen(STUDENT_NUMBER));
        sprintf(date, "%02.0f%02.0f%i", (float) exec_time->tm_mday, (float) exec_time->tm_mon + 1.0, exec_time->tm_year + 1900);
        sprintf(time, "%02.0f%02.0f", (float) exec_time->tm_hour, (float) exec_time->tm_min);
        sprintf(buffer, "%s_%s_%s_%s.out", STUDENT_NUMBER, date, time, op_to_string());
        filename = strdup(buffer);
        free(buffer); free(date); free(time); free(filename);
    }
    if((config.log_fd = fopen(filename, "w+")) == NULL) {
        perror("location: /parser.c/set_logger()\n");
        return 0;
    }
    if((config.log_filename = strdup(filename)) == NULL) {
        perror("location: /parser.c/set_logger()\n");
        return 0;
    }
    return 1;
}
