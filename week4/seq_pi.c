#include<stdio.h>
#include<sys/time.h>
#include<omp.h>

int main(void) {
    long int numSteps = 1000000;
    struct timeval begin, end;
    double step;
    int i;
    double x,pi,sum=0.0;
    gettimeofday(&begin, NULL);

    step = 1 / (double) numSteps;
    for(i = 0; i < numSteps; i++){
         x = (i+0.5)*step;
         sum += 4.0/(1.0+x*x);
    }
    pi = step*sum;
    gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  -  begin.tv_sec) * 1000000u +
           end.tv_usec - begin.tv_usec) / 1.e6;
    printf("pi=%18.16f and time = %12.10f\n",pi, delta);
    printf("... Program completed.\n");
}
