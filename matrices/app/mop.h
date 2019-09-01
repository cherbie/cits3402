#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

#define NUM_OPERATIONS 7
#define BUFFER_SIZE 250
#define STUDENT_NUMBER "22245091"

//STRUCTURES
enum CMD_OPT {sc, tr, ad, ts, mm};
struct MOP_ARG {
    enum CMD_OPT *operation;
    FILE *in1;
    FILE *in2;
    FILE *log;
};

//GLOBAL VARIABLES
char *arg_options[NUM_OPERATIONS];
struct MOP_ARG config;
struct tm *exec_time;
time_t rawtime;
char *op_str;

//FUNCTION DECLARATIONS
int main(int, char**);
void initialise(void);
char *op_to_string(void);
void print_usage(void);
int parse_cmd(int, char**);
int get_operation(char*);
bool config_is_setup(void);
int set_input_files(int, char**, int);
int set_logger(char*);
