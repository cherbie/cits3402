#include<stdio.h>
#include<time.h>
#include<omp.h>

int main(){
long int numSteps=1000;
double step;
int i;
double x,pi,sum=0.0;

step=1/(double)numSteps;
for(i=0;i<numSteps;i++){
     x=(i+0.5)*step;
     sum+=4.0/(1.0+x*x);
}
pi=step*sum;
printf("pi=%18.16f\n",pi);
}

