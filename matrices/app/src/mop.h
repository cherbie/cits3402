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
//#define INPUT_NUM_LINES 3

//STRUCTURES
enum CMD_OPT {sc, tr, ad, ts, mm, f, l}; //MATRIX OPERATIONS
enum MTX_TYPE {int, float};
struct APP_ARG {
    enum CMD_OPT *operation;
    FILE *in1_fd;
    char *in1_filename;
    FILE *in2_fd;
    char *in2_filename;
    FILE *log_fd;
    char *log_filename;
};

struct COO {
    enum MTX_TYPE *type;
    void * mtx;
    int row;
    int col;
    int size[2]; //COO.size[0] - number of elements in mtx. COO.size[1] - number of elements in each sub array.
};

//GLOBAL VARIABLES
char *arg_options[NUM_OPERATIONS];
struct APP_ARG config;
struct tm *exec_time;
struct COO *coo_sparse_mtx; //structure containing coordinate format representation of matrix.
time_t rawtime;
char *op_str;
int (*op_func[NUM_OPERATIONS])(); //pointer to operation functions

// -- FUNCTION DECLARATIONS --
int main(int, char**);
int parse_cmd(int, const char**);
//int get_operation(char*);
bool config_is_setup(void);
int set_input_files(int, const char**, const int);
int set_logger(const char*);

// -- HELPER FUNCTIONS --
void print_config(void);
void initialise(void);
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
