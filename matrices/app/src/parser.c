#include "mop.h"

/**
 * Set the config / operation of execution.
 * return 1 to indicate success and 0 to indicate failure
 */
int parse_cmd(const int argc, const char *argv[]) {
    for(int k = 1; k < argc; k++) {
        if(argv[k][0] != '-') continue;
        for(int i = 0; i < NUM_OPERATIONS; i++) {
            if(strcmp(argv[k], arg_options[i]) == 0) {
                if(i < 5) {
                    config.operation = malloc(1 * sizeof(enum CMD_OPT));
                    *config.operation = i;
                }
                else if(i == 5) { //logger specified
                    if(!set_logger(argv[++k])) {
                        perror(NULL);
                        return 0;
                    }
                }
                else {
                    if((k + 1) >= argc) {
                        fprintf(stderr, "Error: No input files specified.\n");
                        return 0;
                    }
                    else if(!set_input_files(++k, argv, argc)) return 0;
                }
            }
        }
    }
    if(config.log_fd == NULL) {
        char *buffer = malloc(BUFFER_SIZE * sizeof(char));
        char *date = malloc(11 * sizeof(char));
        char *time = malloc(5 * sizeof(char));
        strncpy(buffer, STUDENT_NUMBER, strlen(STUDENT_NUMBER));
        sprintf(date, "%02.0f%02.0f%i", (float) exec_time->tm_mday, (float) exec_time->tm_mon + 1.0, exec_time->tm_year + 1900);
        sprintf(time, "%02.0f%02.0f", (float) exec_time->tm_hour, (float) exec_time->tm_min);
        sprintf(buffer, "%s_%s_%s_%s.out", STUDENT_NUMBER, date, time, op_to_string());
        char *filename = strdup(buffer);
        if(!set_logger(filename)) {
            free(buffer);
            free(date);
            free(time);
            free(filename);
            perror(NULL);
            return 0;
        }
        free(buffer);
        free(date);
        free(time);
        free(filename);
    }
    if(!config_is_setup()) return 0;
    return 1;
}


/**
 * Return 1 to indicate success and 0 to indicate failure
 */
bool config_is_setup(void) {
    if(config.operation == NULL) {
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
int set_input_files(int index, const char * argv[], const int argc) {
    for(int i = 0; i < 2; i++) {
        index += i;
        if(i == 0) {
            if(index >= argc || argv[i][0] == '-') {
                fprintf(stderr, "Error: No input files specified.\n");
                return 0;
            }
            if((config.in1_fd = fopen(argv[index], "r")) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            if((config.in1_filename = strdup(argv[index])) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            continue;
        }
        else if(i == 1) {
            if(index >= argc || argv[index][0] == '-') break;
            if((config.in2_fd = fopen(argv[index], "r")) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            if((config.in2_filename = strdup(argv[index])) == NULL) {
                perror("location: /parser.c/set_input_files()\n");
                return 0;
            }
            break;
        }
    }
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int set_logger(const char *filename) {
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
