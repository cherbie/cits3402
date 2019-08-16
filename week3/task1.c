#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SIZE_ARRAY 100000

float arr[SIZE_ARRAY];

void initLargeArray(void) {
    for(int i = 0; i < SIZE_ARRAY; i++) {
        arr[i] = (float) rand();
    }
    printf("%f", arr[255]);
}

int main(int argc, char *argv[]) {
    time_t start = time(NULL);
    clock_t start_t, end_t;

    initLargeArray();
    float sum = 0;
    float localsum = 0;


    #pragma omp parallel private(localsum)
    {
        int thread_num = omp_get_thread_num();
        printf("%d :: Thread executed.\n", thread_num);
        #pragma omp for
        for(int i = 0; i < SIZE_ARRAY; i++)
        {
                localsum += arr[i];
        }
        sum += localsum; //transfer local memory to sequential scope variable
    }

    printf("\nsum = %.1f\n\n", sum);

    time_t end = time(NULL);
    printf("%d :: Time difference = %2.1f\n", omp_get_thread_num(), difftime(end, start));

    fprintf(stdout, "\nProgram finished running.\n");
    exit(EXIT_SUCCESS);
}
