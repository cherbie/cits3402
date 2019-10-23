#include "sp.h"

/**
 * Find the all pairs shortest path of a given graph.
 * @return matrix of integers containing the shortest paths.
 */
int compute_apsp(SP_CONFIG *config, PATHS *paths) {
    int pweight, sweight; // previous-weight & sum-weight respectively
    int rows = (*paths).nodes;
    int **prev = malloc(rows * sizeof(int  *));
    int **next = NULL;
    if(dup_matrix(&prev[0], (*paths).weight, &rows)) return -1;
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
    int i, j, k;
    int owner; // Process controlling row to be bcast
    int worker; // focus the work performed by nodes
    int* tmp; // Holds the broadcast row

    tmp = (int *) malloc((*paths).nodes * sizeof(int)); // row
    if(tmp == NULL) {
        perror(NULL);
        return -1;
    }

    for (k = 0; k < (*paths).nodes; k++) {
        owner = get_block_owner(k,(*config).nproc, (*paths).nodes); // receiving things? ROW-WISE DISTRIBUTION
        printf("k-value: %i vs owner: %i\n", k, owner);
        if (owner == (*config).rank) { // if the current process is the owner.
            //offset = k - BLOCK_LOW(id,p,n);
            for (j = 0; j < (*paths).nodes; j++)
                tmp[j] = (*paths).weight[k][j]; // contains a column
        }
        MPI_Bcast(tmp, (*paths).nodes, MPI_INT, owner, MPI_COMM_WORLD); // broadcast to all nodes worker nodes what owner contains
        for (i = 0; i < (*paths).nodes; i++) {
            worker = get_worker(i, (*config).nproc, (*paths).nodes); // workers metadata
            printf("row: %i vs worker: %i\n", i, worker);
            if((*config).rank != worker) continue;
            for (j = 0; j < (*paths).nodes; j++)
                (*paths).weight[i][j] = min_weight((*paths).weight[i][j], (*paths).weight[i][k] + tmp[j]); // primitive distributed task
        }
        /* ALL WORKERS BLOCKS OF RESPONSIBILITY ARE UPDATED AT THIS POINT */
    }
    free(tmp);
    return 0;
}

/**
 * Calculates the owner of a row.
 * The owner is considered to be the node contain the most up-to-date shortest path of that row.
 */
int get_block_owner(int k, int p, int n) {
    if(n == p) return ROOT;
    int remainder = n % (p-1); // remainder of even block distribution
    int rowsperblk = (n - remainder) / p; // number of rows handled by each node
    int owner = k/rowsperblk;

    if(owner >= p) return ROOT;
    else {
        owner++;
        if(owner >= p) return ROOT;
        else return owner;
    }
}

/*
 * @return the rank of the block / row owner
 */
int get_worker(int i, int p, int n) {
    if(n == p) return ROOT;
    int remainder = n % (p-1); // remainder of even block distribution
    int rowsperblk = (n - remainder) / p; // number of rows handled by each node
    int worker = (i/rowsperblk);

    if(worker >= p) return ROOT;
    else {
        worker++;
        if(worker >= p) return ROOT;
        else return worker;
    }
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
 *
 *
int collect_final_sp(SP_CONFIG * config, PATHS *paths) {
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
    else {
        MPI_Send(&node_out, PATH_WEIGHT_ARR_SIZE, MPI_INT, ROOT, 0, MPI_COMM_WORLD); // send to root
        MPI_Send(0, 0, MPI_INT, ROOT, FINISHED, MPI_COMM_WORLD); // send finish processing signal
    }
}*/
