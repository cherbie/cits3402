#include "sp.h"

/**
 * Dealloc config struct metadata.
 */
void dealloc_config(SP_CONFIG *config) {
    MPI_File_close(&(*config).file_in);
    free((*config).filename_in);
    free(config);
}
