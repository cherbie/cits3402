#!/bin/sh
clear
mpirun --hostfile hostfile ./sp -f ./examples/32.in
#mpirun ./sp -f "./examples/32.in"
