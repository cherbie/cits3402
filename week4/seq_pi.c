#include<stdio.h>
#include<time.h>
#include<omp.h>

int main(void) {
    long int numSteps = 100000000;
    clock_t begin, end;
    double step;
    int i;
    double x,pi,sum=0.0;
    begin = clock();

    step = 1 / (double) numSteps;
    for(i = 0; i < numSteps; i++){
         x = (i+0.5)*step;
         sum += 4.0/(1.0+x*x);
    }
    pi = step*sum;
    end = clock();
    printf("pi=%18.16f and time = %f\n",pi, (double)(end-begin)/CLOCKS_PER_SEC);
    printf("... Program completed.\n");
}
