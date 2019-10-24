#!/bin/bash
clear
mpirun ./sp -f ./examples/4.in
echo "... completed 4"
mpirun ./sp -f ./examples/16.in
echo "... completed 16"
mpirun ./sp -f ./examples/32.in
echo "... completed 32"
mpirun ./sp -f ./examples/512.in
echo "... completed 512"
mpirun ./sp -f ./examples/1024.in
echo "... completed 1024"
mpirun ./sp -f ./examples/2048.in
echo "... completed 2048"
mpirun ./sp -f ./examples/4096.in
echo "... completed 4096"
echo "... script complete"
