#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <errno.h>

#define NUM_OPERATIONS 5

//GLOBAL VARIABLES
char arg_operations[NUM_OPERATIONS][4];

//FUNCTION DECLARATIONS
int main(int, char**);
void initialise(void);
void print_usage(void);
int parse_cmd(char **);
int get_operation(char *);