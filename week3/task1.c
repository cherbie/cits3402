#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SIZE_ARRAY 1000

float[] arr = [SIZE_ARRAY];

int initLargeArray() {
    
}

int main(int argc, char *argv[]) {
    time_t start = time(NULL);
    #pragma omp parallel private(localsum)
    {
        int thread_num = omp_get_thread_num();
        printf("%d :: Thread executed.\n", thread_num);
        #pragma omp for
        {
            float localsum = 0;
            for(int i = 0; i < SIZE_ARRAY; i++) {
                localsum +=
            }
        }
    }

    time_t end = time(NULL);
    printf("%d :: Time difference = %f\n", omp_get_thread_num(), difftime(end, start));

    fprintf(stdout, "\nProgram finished running.\n");
    exit(EXIT_SUCCESS);
}
