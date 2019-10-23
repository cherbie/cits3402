#include "sp.h"

/**
 * Find the all pairs shortest path of a given graph.
 * @return matrix of integers containing the shortest paths.
 */
int compute_apsp(SP_CONFIG *config, PATHS *paths) {
    int pweight, sweight; // previous-weight & sum-weight respectively
    int rows = (*paths).nodes;
    int **prev = malloc(rows * sizeof(int  *));
    int **next = malloc(rows * sizeof(int *));
    if(dup_matrix(&prev[0], (*paths).weight, &rows)) return -1;
    if(create_matrix(next, &rows)) return -1;
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

/**
 * spributed shortest path calucation using the floyd-warshall algorithm.
 * @return 0 to indicate success and -1 to indicate failure.
 */
int block_apsp(SP_CONFIG *config, PATHS *paths) {
    #define PATH_WEIGHT_ARR_SIZE 3
    #define FINISHED 2

    MPI_Status status;
    int end = 0;
    int block_rem = (*paths).nodes % ((*config).nproc - 1); // remainder of even block distribution
    int block_rows = ((*paths).nodes - block_rem) / ((*config).nproc - 1); // number of rows handled by each node
    int recv_arr[PATH_WEIGHT_ARR_SIZE];
    int **sp = malloc((*paths).nodes * sizeof(int *)); //
    if(sp == NULL) {
        perror(NULL);
        return -1;
    }
    if(dup_matrix(&sp[0], (*paths).weight, &(*paths).nodes)) return -1;
    if((*config).rank == ROOT) {
        do {
            MPI_Recv(&recv_arr, PATH_WEIGHT_ARR_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == FINISHED)
                end++;
            else if(sp[recv_arr[1]][recv_arr[2]] > recv_arr[0]) // new path weight is smaller
                sp[recv_arr[1]][recv_arr[2]] = recv_arr[0];
        } while(end < ((*config).nproc-1)); // receive all processes finish signal (status.MPI_TAG = FINISHED)
        //t2 = MPI_Wtime(); // timing
        print_matrix(sp, &(*paths).nodes); // printout again the new array with shortest paths
        (*paths).sp = sp;
        //printf("total time %f \n",t2-t1);
    }
    else { // slaves
        int i, j, k;
        int node_out[PATH_WEIGHT_ARR_SIZE];

        if((*config).rank+1 != (*config).nproc) block_rem = 0; // last rank block does more work
        for (k = block_rows*((*config).rank-1); k < block_rows*((*config).rank-1)+block_rows+block_rem; ++k) // last node performs final block size
            for (i = 0; i < (*paths).nodes; ++i)
                for (j = 0; j < (*paths).nodes; ++j)
                    if ((sp[i][k] * sp[k][j] != 0) && (i != j)) // path does exist
                        if ((sp[i][k] + sp[k][j] < sp[i][j]) || (sp[i][j] == 0)) { // shorter path exists
                            sp[i][j] = sp[i][k] + sp[k][j];
                            node_out[0] = sp[i][j];
                            node_out[1] = i;
                            node_out[2] = j;
                            MPI_Send(&node_out, PATH_WEIGHT_ARR_SIZE, MPI_INT, ROOT, 0, MPI_COMM_WORLD); // send to root
                        }
        MPI_Send(0, 0, MPI_INT, ROOT, FINISHED, MPI_COMM_WORLD); // send finish processing signal
    }
    return 0;
}


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
    int *tmp;
    int **next;

    printf("10\n");

    tmp = calloc((*paths).nodes, sizeof(int)); // hold the broadcasted row.
    if(tmp == NULL) {
        perror(NULL);
        return -1;
    }
    printf("11");

    if(prep_weights((*paths).weight, &(*paths).nodes)) {
        perror(NULL);
        return -1;
    }
    printf("12");

    next = calloc((*paths).nodes, sizeof(int *));
    if(create_matrix(next, &(*paths).nodes)) {
        perror(NULL);
        return -1;
    }
    printf("14");
    if((*config).rank == ROOT) {
        printf("starting ...\n");
        print_matrix((*paths).weight, &(*paths).nodes);
    }
    for (k = 0; k < (*paths).nodes; k++) {
        owner = get_block_owner(k,(*config).nproc, (*paths).nodes); // receiving things? ROW-WISE DISTRIBUTION
        printf("k-value: %i vs owner: %i\n", k, owner);
        if (owner == (*config).rank) { // if the current process is the owner.
            for (j = 0; j < (*paths).nodes; j++)
                tmp[j] = (*paths).weight[k][j]; // contains a column
        }
        MPI_Bcast(tmp, (*paths).nodes, MPI_INT, owner, MPI_COMM_WORLD); // broadcast to all nodes worker nodes what owner contains
        printf("k value = %i ... ", k);
        //print_array(&tmp, &(*paths).nodes);
        for (i = 0; i < (*paths).nodes; i++) {
            worker = get_worker(i, (*config).nproc, (*paths).nodes); // workers metadata
            printf("row: %i vs worker: %i\n", i, worker);
            if((*config).rank != worker) continue;
            for (j = 0; j < (*paths).nodes; j++) {
                if(i == j) next[i][j] = 0;
                else next[i][j] = min_weight((*paths).weight[i][j], (*paths).weight[i][k] + tmp[j]); // primitive distributed task
            }
        }
        if(cpy_matrix((*paths).weight, next, &(*paths).nodes)) {
            fprintf(stderr, "Problem determining shortest path.\n");
            return -1;
        }
        if((*config).rank == ROOT) {
            printf(" -- matrix at k = %i -- \n", k);
            print_matrix((*paths).weight, &(*paths).nodes);
        }
        /* ALL WORKERS BLOCKS OF RESPONSIBILITY ARE UPDATED AT THIS POINT */
    }
    free(tmp);
    free_mtx((void **) next, &(*paths).nodes);
    return 0;
}

/**
 * Calculates the owner of a row.
 * The owner is considered to be the node contain the most up-to-date shortest path of that row.
 */
int get_block_owner(int k, int p, int n) {
    printf(" ... calculate block owner\n");
    //if(n == p) return ROOT;
    int remainder = n % p; // remainder of even block distribution
    int rowsperblk = (n - remainder) / p; // number of rows handled by each node
    int owner = k/rowsperblk;

    owner++;
    if(owner >= p) return ROOT; // ROOT CONTAINS THE REMAINDER
    else return owner;
}

/*
 * @return the rank of the block / row owner
 */
int get_worker(int i, int p, int n) {
    //if(n == p) return ROOT;
    printf("calculate block owner\n");
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
    int offset = 0;
    int nelememts = 10;
    int recvcounts[(*config).nproc] = {23, 123}; // integer array containing number of elements to be received.
    int displs[(*config).nproc] = {123, 0, 0};

    // GATHER ALL DATA
    MPI_Gatherv(&(*config).mtx[offset][0], nelements, MPI_INT, &(*paths).sp[0][0], recvcounts, displs, MPI_INT, ROOT, MPI_COMM_WORLD);

    // ROOT  SHOULD CONTAIN ALL SHORTEST PATHS AT THIS STAGE.

    return 0;
}
