#include "sp.h"

/**
 *
 * @param id is the the master process.
 * @param p is the number of processes
 * @param a is the shortest path
 * @param n is the number of rows in adjacency matrix.
 * NOTE: need to broadcast all contained values in node to master before final processing.
 */
int compute_shortest_paths(SP_CONFIG *config, PATHS *paths) {
    int i = 0, j = 0, k = 0;
    int owner; // Process controlling row to be bcast
    int worker; // focus the work performed by nodes
    int offset; // ROW STARTING INDEX

    int *tmp = calloc((*paths).nodes, sizeof(int)); // hold the broadcasted row.
    if(tmp == NULL) {
        perror(NULL);
        return -1;
    }

    // -- SET UNDEFINED WEIGHTS --
    if(prep_weights((*paths).weight, &(*paths).nodes)) {
        perror(NULL);
        return -1;
    }

    int *next = calloc((*paths).nodes * (*paths).nodes, sizeof(int));
    if(next == NULL) {
        perror(NULL);
        return -1;
    }

    /*if((*config).rank == ROOT) {
        printf("starting ...\n");
        print_matrix((*paths).weight, &(*paths).nodes);
    }*/

    // -- INITIALISE EXECUTION TIME LOGGING --
    if((*config).rank == ROOT)
        if(set_time_logger(config, paths)) return -1;
    
    // -- START TIMER --
    (*config).starttime = MPI_Wtime();

    // -- DISTRIBUTED ALGORITHM --
    for (k = 0; k < (*paths).nodes; k++) {
        owner = get_block_owner(k,(*config).nproc, (*paths).nodes); // receiving things? ROW-WISE DISTRIBUTION

        if (owner == (*config).rank) { // if the current process is the owner.
            offset = k * (*paths).nodes; // start index of row
            for (j = 0; j < (*paths).nodes; j++)
                tmp[j] = (*paths).weight[offset + j]; // contains a column
        }

        MPI_Bcast(tmp, (*paths).nodes, MPI_INT, owner, MPI_COMM_WORLD); // broadcast to all worker nodes what owner contains

        //printf("k value = %i ... ", k);
        for (i = 0; i < (*paths).nodes; i++) {
            worker = get_worker(i, (*config).nproc, (*paths).nodes); // workers metadata
            //printf("row: %i vs worker: %i\n", i, worker);

            if((*config).rank != worker) continue; // NOT YOUR WORK TO BE DONE !

            offset = i * (*paths).nodes; // start index of row

            for (j = 0; j < (*paths).nodes; j++) {
                if(i == j) next[offset + j] = 0;
                else next[offset + j] = min_weight((*paths).weight[offset+j], (*paths).weight[offset + k] + tmp[j]); // primitive distributed task
            }
        }
        memcpy((void *) (*paths).weight, (void *) next, ((*paths).nodes * (*paths).nodes) * sizeof(int));
        /*if((*config).rank == ROOT) {
            printf(" -- matrix at k = %i -- \n", k);
            print_matrix((*paths).weight, &(*paths).nodes);
        }*/
        /* ALL WORKERS BLOCKS OF RESPONSIBILITY ARE UPDATED AT THIS POINT */
    }

    // -- COLLECT DATA FROM DISTRIBUTED NODES --
    if(collect_final_sp(config, paths)) {
        fprintf(stderr, "Error collating computed shortest paths.\n");
        return -1;
    }

    // -- END TIMER --
    (*config).endtime = MPI_Wtime();

    // -- DEALLOCATE MEMORY --
    free(tmp);
    free(next);
    free((*paths).weight);

    // -- LOG RESULT TO FILE --
    if((*config).rank == ROOT) {
        if(log_result(config, paths)) {
            fprintf(stderr, "An error has occured logging the all pairs shortest paths.\n");
            return -1;
        }
        if(log_time_analysis(config, paths)) {
            fprintf(stderr, "Error logging execution time analysis to file.\n");
        }
    }

    return 0;
}

/**
 * Calculates the owner of a row.
 * The owner is considered to be the node contain the most up-to-date shortest path of that row.
 */
int get_block_owner(int k, int p, int n) {
    //printf(" ... calculate block owner\n");
    //if(n == p) return ROOT;
    int remainder = n % p; // remainder of even block distribution
    int rowsperblk = (n - remainder) / p; // number of rows handled by each node
    int owner = k/rowsperblk;

    owner++;
    if(owner >= p) return ROOT; // ROOT CONTAINS THE REMAINDER
    else return owner;
}

/**
 * @return the starting index of block owners units / weights.
 */
int get_merge_info(SP_CONFIG* config, PATHS* paths, int *offset, int *nelem, int **recvcounts, int **displ) {
    //printf("... merge info\n");
    int remainder = (*paths).nodes % (*config).nproc; // remainder of even block distribution
    int rowsperblk = ((*paths).nodes - remainder) / (*config).nproc; // number of rows handled by each node

    if((*config).rank == ROOT) {
        *offset = (rowsperblk * (*paths).nodes) * ((*config).nproc - 1);
        *nelem = (rowsperblk + remainder) * (*paths).nodes;
    }
    else {
        *offset = (rowsperblk * (*paths).nodes) * ((*config).rank - 1);
        *nelem = rowsperblk * (*paths).nodes;
    }
    for(int i = 0; i < (*config).nproc; i++) { // RANK OF THE PROCESS
        if(i == ROOT) {
            (*recvcounts)[i] = (rowsperblk + remainder) * (*paths).nodes;
            (*displ)[i] = (rowsperblk * (*paths).nodes) * ((*config).nproc - 1);
            continue;
        }
        (*recvcounts)[i] = rowsperblk * (*paths).nodes;
        (*displ)[i] = (rowsperblk * (*paths).nodes) * (i - 1);
    }
    return 0;
}

/*
 * @return the rank of the block / row owner
 * IMPROVEMENT USE VALUES IN CONFIG AND PATHS.
 */
int get_worker(int i, int p, int n) {
    //printf("calculate block owner\n");
    int remainder = n % p; // remainder of even block distribution
    int rowsperblk = (n - remainder) / p; // number of rows handled by each node
    int worker = (i/rowsperblk);

    worker++;
    if(worker >= p) return ROOT; // ROOT CONTAINS THE REMAINDER
    else return worker;
}

/**
 * Calculate the minimum weight / shortest path of the provided parameters.
 */
int min_weight(int original, int new) {
    if(original <= new) return original;
    else return new;
}

/**
 * Collect the shortest path metadata held by various distributed nodes.
 * based on their process rank. Send this data to the master rank.
 * Send an array of integers.
 */
int collect_final_sp(SP_CONFIG *config, PATHS *paths) {
    if((*config).rank == ROOT) {
        (*paths).sp = malloc(((*paths).nodes * (*paths).nodes) * sizeof(int));
        if((*paths).sp == NULL) {
            perror(NULL);
            return -1;
        }
    }

    int offset, nelements;
    int *recvcounts = malloc((*config).nproc * sizeof(int *));
    int *displs = malloc((*config).nproc * sizeof(int *));
    if(recvcounts == NULL || displs == NULL) {
        perror(NULL);
        return -1;
    }
    if(get_merge_info(config, paths, &offset, &nelements, &recvcounts, &displs)) {
        fprintf(stderr, "Error merging distributed data\n");
        return -1;
    }

    // -- GATHER ALL DATA AT ROOT --
    MPI_Gatherv(&(*paths).weight[offset], nelements, MPI_INT, &(*paths).sp[0], recvcounts, displs, MPI_INT, ROOT, MPI_COMM_WORLD);

    // ROOT SHOULD CONTAIN ALL SHORTEST PATHS AT THIS STAGE.

    //printf("... COMPLETED MERGE\n");
    return 0;
}
