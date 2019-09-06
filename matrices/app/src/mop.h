#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>

#define NUM_OPERATIONS 5
#define NUM_OPTIONS 7
#define BUFFER_SIZE 250
#define STUDENT_NUMBER "22245091"
#define NUMBER_OF_INPUT_FILES 2

//#define INPUT_NUM_LINES 3
typedef struct {
    int i;
    float f;
} MTX_TYPE;
// -- STRUCTURES --
struct APP_ARG {
    int  operation;
    MTX_TYPE sm; //scalar number
    int  num_threads;
    FILE **fd;
    char **filename;
    FILE *in2_fd;
    char *in2_filename;
    FILE *log_fd;
    char *log_filename;
};
typedef struct {
    //enum MTX_TYPE *type;
    char *type;
    void *mtx;
    int row;
    int col;
    int size[2]; //COO.size[0] - number of elements in mtx. COO.size[1] - number of elements in each sub array.
} COO;

//GLOBAL VARIABLES
struct APP_ARG config;
struct tm *exec_time;
COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
time_t rawtime;
char *op_str;
char *arg_options[NUM_OPTIONS];
int (*op_func[NUM_OPERATIONS])(); //pointer to operation functions


// -- FUNCTION DECLARATIONS --
int main(int, char**);
int parse_cmd(int, char**);
int get_operation(char*);
bool config_is_setup(void);
int set_input_files(int, char*);
int set_logger(void);
int set_config_threads(char*);
int set_config_sm(char*);

// -- HELPER FUNCTIONS --
void print_config(void);
int initialise(void);
char *op_to_string(void);
void print_usage(void);
void print(char *);

// -- OPERATIONS --
int operation_main(void);
int scalar_mp(void);
int trace(void);
int addition(void);
int transpose_matrix(void);
int matrix_mp(void);
int read_in_mtx(char**, int, int, int, const int*);
