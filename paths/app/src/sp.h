#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <getopt.h>

#define _POSIX_C_SOURCE 2

#define ROOT 0
#define STUCT_PATH_NUM_ELEM 3
#define STRUCT_PATH_TYPE {MPI_INT, MPI_INT, MPI_INT}
#define STRUCT_PATH_DISP {0, sizeof(int), sizeof(int)}
#define LARGE_INT 9999999
#define STUDENT_NUMBER 22245091

// -- DATA STRUCTURES --

typedef struct {
    int nproc;
    int rank;
    char *filename_in;
    MPI_File file_in;
    //FILE *fp_in;
    char *filename_out;
    FILE *fp_out;
    char *time_out; // time execution analysis
    FILE *fp_time_out; // time execution analysis.
    double starttime;
    double endtime;
} SP_CONFIG;

// -- FOR REFERENCE --
typedef struct {
    int nodes;
    int *weight; // weight of path.
    int *sp;
} PATHS;


// -- FUNCTIONS --
extern int main(int, char**);
extern int initialise(SP_CONFIG*, PATHS*);


// -- PARSING FUNCTIONS --
extern int parse_args(SP_CONFIG*, int*, char***);
extern int set_logger(SP_CONFIG*);
extern int set_time_logger(SP_CONFIG*, PATHS*);
extern int log_result(SP_CONFIG*, PATHS*);
extern int log_time_analysis(SP_CONFIG*, PATHS*);


// -- HELPER FUNCTIONS --
extern void print_matrix(int*, int *);
extern int create_matrix(int **, int *);
extern int prep_weights(int*, int *);
extern int dup_matrix(int **, int **, int *);
extern int cpy_matrix(int **, int **, int *);
extern void print_array(int **, int *);


// -- FILE READING --
extern int read_file_mpi(SP_CONFIG*, PATHS*);

// -- CALCULATIONS --
extern int find_apsp(SP_CONFIG *, PATHS *);
extern int block_apsp(SP_CONFIG *, PATHS *);
extern int compute_shortest_paths(SP_CONFIG *, PATHS *);
extern int get_block_owner(int, int, int);
extern int get_worker(int, int, int);
extern int min_weight(int, int);
extern int collect_final_sp(SP_CONFIG*, PATHS*);
extern int get_merge_info(SP_CONFIG*, PATHS*, int*, int*, int**, int**);

// -- ERRORS --
extern void error_handler(int*);


// -- DEALLOC --
extern void dealloc_config(SP_CONFIG*);
