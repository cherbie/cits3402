#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#define NUM_OPERATIONS 5
#define NUM_OPTIONS 7
#define MAX_MTX_DIMENSIONS 10
#define BUFFER_SIZE 250
#define STUDENT_NUMBER "22245091"
#define NUMBER_OF_INPUT_FILES 2

//#define INPUT_NUM_LINES 3
typedef struct {
    int   i;
    float f;
} MTX_TYPE;

// -- STRUCTURES --
typedef struct {
    int  operation;
    char *op_str;
    float sm; //scalar number
    int  num_threads;
    FILE **fd;
    char **filename;
    FILE *log_fd;
    char *log_filename;
} CONFIG;

typedef struct {
    //enum MTX_TYPE *type;
    bool is_int;
    int **mtxi;
    float **mtxf;
    int row;
    int col;
    int size; //COO.size[0] - number of elements in mtx. COO.size[1] - number of elements in each sub array.
} COO;

typedef struct {
    bool is_int;
    int *mtxi; //non-zero values
    float *mtxf; //non-zero values
    int *mtx_offset; //stores number of non-zero elements in each row.
    int *mtx_col; //stores the column index of each non-zero element
    int size; //number of non-zero input elements
    int row; //number of rows specified by the input file
    int col; //number of columns specified by the input file
} CSR;

typedef struct {
    bool is_int;
    int *mtxi; //non-zero values
    float * mtxf; //non-zero values
    int *mtx_offset; //number of non-zero values in each row
    int *mtx_col; //stores the column index of each non-zero element
    int size; //number of non-zero input elements
    int row; //number of rows specified by the input file
    int col; //nuumber of columns specified by the input file
} CSS;

//GLOBAL VARIABLES
CONFIG          config;
struct tm       *exec_time;
time_t          rawtime;
char            *arg_options[NUM_OPTIONS];
int             (*op_func[NUM_OPERATIONS])(); //pointer to operation functions


// -- FUNCTION DECLARATIONS --
extern int main(int, char**);
extern int parse_cmd(int, char**);
extern int get_operation(char*);
extern bool config_is_setup(void);
extern int set_input_files(int, char*);
extern int set_logger(void);
extern int set_config_threads(char*);
extern int set_config_sm(char*);

// -- HELPER FUNCTIONS --
extern void print_config(void);
extern int initialise(void);
extern char *op_to_string(void);
extern void print_usage(void);
extern void print(char *);
extern char *str_clean(char*);
extern void print_coo(COO*);
extern bool is_defined(COO*, int, int, int);
extern void print_csr(CSR*);
extern void print_css(CSS*);

// -- OPERATIONS --
extern int operation_main(void);
extern int scalar(void);
extern int trace(void);
extern int addition(void);
extern int transpose_matrix(void);
extern int matrix_mp(void);

// -- FILE READING --
extern int read_to_coo(COO **, int);
extern int read_to_csr(CSR **, int);
extern int read_coo_filei(COO**,int);
extern int read_coo_filef(COO**, int);
extern int add_int_coo(COO*,int,int,int,int);
extern int add_float_coo(COO*,float,int,int,int);
extern int read_csr_filei(CSR **, int);
extern int read_csr_filef(CSR**, int);
extern int add_int_csr(CSR*, int, int, int, int);
extern int add_float_csr(CSR*, float, int, int, int);


// -- SYNCHRONOUS --
extern int process_scalar(COO*, float);
extern int process_trace(COO*,int*,float*);
extern int process_addition(COO**);
extern int process_transpose(CSR*, CSS*);

// -- LOGGER --
extern int log_coo_result(COO*);
extern int log_trace_result(COO*, int*, float*);
extern int log_transpose_result(CSS *);

// -- DEALLOC --
extern void dealloc_coo(COO**,int);
extern void dealloc_config(void);
extern void dealloc_csr(CSR**, int);
extern void dealloc_css(CSS**, int);
