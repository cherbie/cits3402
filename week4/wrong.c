#include<stdio.h>
#include<sys/time.h>
#include<omp.h>

int main() {
    struct timeval begin, end;
    long int numSteps = 10000;
    int i;
    double x,pi,sum=0.0;
    double step = 1/(double)numSteps;
    gettimeofday(&begin, NULL);
    omp_set_num_threads(4); //I have set the number of threads =4, you can change this.
    #pragma omp parallel private(x)
    {
        i = 0;
        for(i = 0; i < numSteps; i++){
           x = (i+0.5)*step;
           sum += 4.0/(1.0+x*x);
        }
        pi = sum*step;
        gettimeofday(&end, NULL); //clock() is not threading safe
        float delta = ((end.tv_sec  - begin.tv_sec) * 1000000u +
           end.tv_usec - begin.tv_usec) / 1.e6;
        printf("pi=%18.16f and time =%f\n",pi, delta);
    }
}
