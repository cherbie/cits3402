#!/bin/bash
clear
mpirun --hostfile hostfile -np 1 ./sp -f ./examples/2048.in
echo "... completed p = 1"
mpirun --hostfile hostfile -np 2 ./sp -f ./examples/2048.in
echo "... completed p = 2"
mpirun --hostfile hostfile -np 4 ./sp -f ./examples/2048.in
echo "... completed p = 4"
mpirun --hostfile hostfile -np 6 ./sp -f ./examples/2048.in
echo "... completed p = 6"
mpirun --hostfile hostfile -np 8 ./sp -f ./examples/2048.in
echo "... completed p = 8"
mpirun --hostfile hostfile -np 10 ./sp -f ./examples/2048.in
echo "... completed p = 10"
mpirun --hostfile hostfile -np 12 ./sp -f ./examples/2048.in
echo "... completed p = 12"
mpirun --hostfile hostfile -np 14 ./sp -f ./examples/2048.in
echo "... completed p = 14"
mpirun --hostfile hostfile -np 16 ./sp -f ./examples/2048.in
echo "... completed p = 16"
echo "... script complete"
