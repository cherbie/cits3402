#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define ROOT 0

// -- DATA STRUCTURES --
typedef struct {
    int size;
    int rank;
    char *filename_in;
    MPI_File file_in;
    char *filename_out;
    MPI_File file_out;
} SP_CONFIG;

// -- FUNCTIONS --
extern int main(int, char**);
extern SP_CONFIG *set_config(void);

// -- PARSING FUNCTIONS --
extern int parse_args(SP_CONFIG*, int*, char***);

// -- FILE READING --
extern int read_input(SP_CONFIG*);
extern int create_output(SP_CONFIG*, char*);

// -- ERRORS --
extern void error_handler(int*);

// -- DEALLOC --
extern void dealloc_config(SP_CONFIG*);
