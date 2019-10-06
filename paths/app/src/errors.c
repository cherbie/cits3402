#include "sp.h"

void error_handler(int *status) {
    char err_str[MPI_MAX_ERROR_STRING];
    int err_len;
    MPI_Error_string(*status, err_str, &err_len);
    fprintf(stderr, "%s\n", err_str);
}
