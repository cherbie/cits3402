#!/bin/sh
clear
#mpirun --hostfile hostfile ./sp -f ./examples/32.in
mpirun -n 3 ./sp -f "./examples/2048.in"
