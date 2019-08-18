#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SIZE_ARRAY 10000
//#define NUM_THREADS 1

float arr[SIZE_ARRAY];

void initLargeArray(void) {
    for(int i = 0; i < SIZE_ARRAY; i++) {
        arr[i] = (float) abs(rand());
    }
    printf("%f\n", arr[255]);
    printf("%f\n", arr[1120]);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: ./t1 NUM_THREADS\n");
        exit(EXIT_FAILURE);
    }
    clock_t start_t, end_t, total_t;

    start_t = clock();
    int num_threads = atoi(argv[1]);
    float sum = 0;
    float localsum = 0;

    initLargeArray();
    omp_set_num_threads(num_threads);

    #pragma omp parallel private(localsum)
    {
        int thread_num = omp_get_thread_num();
        printf("%d :: Thread executed.\n", thread_num);
        #pragma omp for
        for(int i = 0; i < SIZE_ARRAY; i++)
        {
                localsum += arr[i];
        }
        printf("localsum = %f\n", localsum);
        sum += localsum; //transfer local memory to sequential scope variable
    }

    printf("-- > localsum = %f\n", localsum);
    printf("\nsum = %.1f\n\n", sum);
    end_t = clock();
    total_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%d :: Time difference = %ld \n", omp_get_thread_num(), total_t);



    fprintf(stdout, "\nProgram finished running.\n");
    exit(EXIT_SUCCESS);
}
