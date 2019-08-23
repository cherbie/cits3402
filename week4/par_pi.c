#include<stdio.h>
#include<sys/time.h>
#include<omp.h>

#define NUM_THREADS 4

int main() {
    struct timeval begin, end;
    long int numSteps = 100000000;
    //int i = 0;
    double x, pi = 0.0;
    double sums[NUM_THREADS] = {0.0};
    double sum = 0.0;
    double step = 1/(double)numSteps;
    gettimeofday(&begin, NULL);
    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel private(x)
    {
        #pragma omp for
        for(int i = 0; i < numSteps; i++){
           x = (i+0.5)*step;
           sum += 4.0/(1.0+x*x);
        }
        sums[omp_get_thread_num()] = sum;
    }
    //MASTER
    for(int i = 0; i < NUM_THREADS; i++) {
        pi += sums[i]*step;
    }
    gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  -  begin.tv_sec) * 1000000u +
           end.tv_usec - begin.tv_usec) / 1.e6;
    printf("pi = %18.16f and time = %12.10f\n", pi, delta);
    printf("... Program complete.\n");
}
