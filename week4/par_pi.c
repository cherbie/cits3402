#include<stdio.h>
#include<time.h>
#include<omp.h>

#define NUM_THREADS 8

int main() {
    clock_t begin, end;
    long int numSteps = 100000000;
    double step;
    int i = 0;
    double x, pi = 0.0;
    double sum[NUM_THREADS] = {0.0};
    step = 1/(double)numSteps;
    begin = clock();
    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel for
        for(i = 0; i < numSteps; i++){
           x = (i+0.5)*step;
           sum[omp_get_thread_num()] += 4.0/(1.0+x*x);
        }
    #pragma omp master
    {
        for(i = 0; i < NUM_THREADS; i++) {
            pi += sum[i]*step;
        }
        end = clock();
        printf("pi = %18.16f and time = %f\n",pi, (double)(end - begin) / CLOCKS_PER_SEC);
    }
    printf("... Program complete.\n");
}
