#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define ROOT 0
#define STUCT_PATH_NUM_ELEM 3
#define STRUCT_PATH_TYPE {MPI_INT, MPI_INT, MPI_INT}
#define STRUCT_PATH_DISP {0, sizeof(int), sizeof(int)}

// -- DATA STRUCTURES --

typedef struct {
    int size;
    int rank;
    char *filename_in;
    MPI_File file_in;
    FILE *fp_in;
    char *filename_out;
    MPI_File file_out;
    FILE *fp_out;
} SP_CONFIG;

// -- FOR REFERENCE --
typedef struct {
    uint16_t nodes;
    uint16_t **weight; // weight of path.
} PATHS;

// -- MPI TYPES --
extern MPI_Datatype Paths;
//extern const MPI_Datatype Path_type[TYPE_PATH_NUM_ELEM];
//extern const MPI_Aint Path_disp[TYPE_PATH_NUM_ELEM];


// -- FUNCTIONS --
extern int main(int, char**);
extern int initialise(SP_CONFIG*, PATHS*);

// -- PARSING FUNCTIONS --
extern int parse_args(SP_CONFIG*, int*, char***);

// -- FILE READING --
extern int read_input(SP_CONFIG*, PATHS*);
extern int create_output(SP_CONFIG*, char*);

// -- ERRORS --
extern void error_handler(int*);

// -- DEALLOC --
extern void dealloc_config(SP_CONFIG*);
extern void dealloc_paths(PATHS*);
