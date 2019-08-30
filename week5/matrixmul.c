#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define COL 3
#define ROW 3
#define SEED 17
#define MAX_INT 5

void matrixToString(int ***matrix, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf(" [%i] ", (*matrix)[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void def_matrix(int ***matrix, int rows, int cols) {
    *matrix = (int **) malloc(rows * sizeof(int *));
    for(int i = 0; i < rows; i++) {
        (*matrix)[i] = (int *) malloc(cols * sizeof(int));
        for(int j = 0; j < cols; j++) {
            (*matrix)[i][j] = (int) rand() % MAX_INT;
        }
    }
}

void init_matrix(int ***matrix, int rows, int cols) {
    *matrix = (int **) malloc(rows * sizeof(int *));
    for(int i = 0; i < rows; i++) {
        (*matrix)[i] = (int *) malloc(cols * sizeof(int));
    }
}

/**
 * Assume correct dimensions for provided matrices.
 * return 1 to indicate failure and 0 to indicate success
 */ 
int multiply(int **res, int **mat1, int **mat2) {
    int sum = 0; //temporary multiplication result
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            for(int k = 0; k < COL; k++) 
                sum += (mat1[i][k] * mat2[k][j]);
            res[i][j] = sum;
            sum = 0;
        }
    }
    return 1;
}

void deallocate(int ***block, int rows) {
    for(int i = 0; i < rows; i++) {
        free((*block)[i]);
    }
    free(*block);
}

int main(int argc, char *argv[]) {
    int **matrix1;
    int **matrix2;
    int **result;
    srand(SEED);
    def_matrix(&matrix1, ROW, COL);
    def_matrix(&matrix2, COL, ROW);
    init_matrix(&result, ROW, ROW);
    if(!multiply(result, matrix1, matrix2)) {
        fprintf(stderr, "Error performing matrix multiplication.\n");
        exit(EXIT_FAILURE);
    }
    printf("----- A ------\n");
    matrixToString(&matrix1, ROW, COL);
    printf("----- B ------\n");
    matrixToString(&matrix2, ROW, COL);
    printf("----- RESULT ------\n");
    matrixToString(&result, ROW, COL);
    deallocate(&matrix1, ROW);
    deallocate(&matrix2, ROW);
    deallocate(&result, ROW);
    printf("\n... Program complete\n");
    exit(EXIT_SUCCESS);
}