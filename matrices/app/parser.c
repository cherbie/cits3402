#include "mop.h"

/**
 * Set the config / operation of execution.
 * return 1 to indicate success and 0 to indicate failure
 */
int parse_cmd(int argc, char *argv[]) {
    for(int k = 1; k < argc; k++) {
        if(argv[k][0] != '-') continue;
        for(int i = 0; i < NUM_OPERATIONS; i++) {
            if(strcmp(argv[k], arg_options[i]) == 0) {
                if(i < 5) *config.operation = i;
                else if(i == 5) { //logger specified
                    if(!set_logger(argv[++k])) {
                        perror(NULL);
                        return 0;
                    }
                }
                else {
                    if((k + 1) >= argc) {
                        fprintf(stderr, "Error: No logger output file specified.\n");
                        return 0;
                    }
                    if(!set_input_files(++k, argv, argc)) return 0;
                }
            }
        }
    }
    if(config.log == NULL) {
        char *buffer = malloc(BUFFER_SIZE * sizeof(char));
        char *date = malloc(11 * sizeof(char));
        char *time = malloc(5 * sizeof(char));
        strncpy(buffer, STUDENT_NUMBER, strlen(STUDENT_NUMBER));
        sprintf(date, "%i%i%i", exec_time->tm_mday, exec_time->tm_mon + 1, exec_time->tm_year + 1900);
        sprintf(time, "%i%i", exec_time->tm_hour, exec_time->tm_min);
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
    if(config.operation == NULL || config.in1 == NULL || config.log == NULL) {
        fprintf(stderr, "Error: Not enough command line arguments specified.\n");
        return false;
    }
    else return true;
}


/**
 *
 * Return 1 to indicate success and 0 to indicate failure.
 */
int set_input_files(int index, char * argv[], int argc) {
    for(int i = 0; i < 2; i++) {
        index += i;
        if(index >= argc || (argv[i][0] == '-' && i < 1)) {
            fprintf(stderr, "Error: No input files specified.\n");
            return 0;
        }
        else if(argv[i][0] == '-') break;
        else if(i == 0) {
            config.in1 = fopen(argv[i], "r");
            if(config.in1 == NULL) {
                perror(NULL);
                return 0;
            } continue;
        }
        else if(i == 1) {
            config.in2 = fopen(argv[i], "r");
            if(config.in2 == NULL) {
                perror(NULL);
                return 0;
            } break;
        }
    }
    return 1;
}

/**
 * Return 1 to indicate success and 0 to indicate failure.
 */
int set_logger(char *filename) {
    config.log = fopen(filename, "w+");
    if(config.log == NULL) return 0;
    return 1;
}
