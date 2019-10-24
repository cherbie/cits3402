#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define ROOT 0
#define STUCT_PATH_NUM_ELEM 3
#define STRUCT_PATH_TYPE {MPI_INT, MPI_INT, MPI_INT}
#define STRUCT_PATH_DISP {0, sizeof(int), sizeof(int)}
#define LARGE_INT 9999999

// -- DATA STRUCTURES --

typedef struct {
    int nproc;
    int rank;
    char *filename_in;
    MPI_File file_in;
    FILE *fp_in;
    char *filename_out;
    FILE *fp_out;
    char *time_out;
    FILE *fp_time_out;
    double starttime;
    double endtime;
} SP_CONFIG;

// -- FOR REFERENCE --
typedef struct {
    int nodes;
    int **weight; // weight of path.
    int **sp;
} PATHS;


// -- FUNCTIONS --
extern int main(int, char**);
extern int initialise(SP_CONFIG*, PATHS*);


// -- PARSING FUNCTIONS --
extern int parse_args(SP_CONFIG*, int*, char***);
extern int log_time(SP_CONFIG*, PATHS*);


// -- HELPER FUNCTIONS --
extern void print_matrix(int **, int *);
extern int **create_matrix(int *);
extern int prep_weights(int **, int *);
extern int dup_matrix(int **, int **, int *);
extern int cpy_matrix(int **, int **, int *);


// -- FILE READING --
extern int read_file_mpi(SP_CONFIG*, PATHS*);
extern int process_matrix_array(SP_CONFIG*, PATHS*, int*);
extern int create_output(SP_CONFIG*, char*);

// -- CALCULATIONS --
extern int find_apsp(SP_CONFIG *, PATHS *);

// -- ERRORS --
extern void error_handler(int*);


// -- DEALLOC --
extern void dealloc_config(SP_CONFIG*);
extern void dealloc_paths(PATHS*);
extern void free_mtx(void **, int *);
