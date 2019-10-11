#include "sp.h"

/**
 * Find the all pairs shortest path of a given graph.
 * @return matrix of integers containing the shortest paths.
 */
int find_apsp(SP_CONFIG *config, PATHS *paths) {
    int pweight, sweight; // previous-weight & sum-weight respectively
    int rows = (*paths).nodes;
    int **prev = malloc(rows * sizeof(int  *));
    int **next = NULL;
    if(dup_matrix(&prev[0], (*paths).weight, &rows)) return -1;
    if(prep_weights(&prev[0], &rows)) return -1;
    next  = create_matrix(&rows);
    if(next == NULL) return -1; // error allocating memory
    for(int k = 0; k < rows; k++) {
        printf("new k value: %i\n", k);
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < rows; j++) {
                if(i == j) {
                    next[i][j] = 0;
                    continue;
                }
                pweight = prev[i][j];
                sweight = prev[i][k] + prev[k][j];
                if(pweight < sweight) next[i][j] = pweight;
                else next[i][j] = sweight;
            }
        }
        if(k == (rows-1)) break;
        print_matrix(prev, &rows);
        if(cpy_matrix(prev, next, &rows)) {
            fprintf(stderr, "Problem determining shortest path.\n");
            return -1;
        }
    }
    free_mtx((void **)prev, &rows);
    (*paths).sp = next; // shortest path
    return 0;
}
