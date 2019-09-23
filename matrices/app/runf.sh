#!/bin/bash
clear
rm -f float_results.txt
echo "---------- START --------"
./mop --mm -f ./input/float64.in ./input/float64.in -l -s >> float_results.txt
echo " --------- DONE 64 ---------"
./mop --mm -f ./input/float128.in ./input/float128.in -l -s >> float_results.txt
echo " --------- DONE 128 --------"
./mop --mm -f ./input/float256.in ./input/float256.in -l -s >> float_results.txt
echo " --------- DONE 256 ---------"
./mop --mm -f ./input/float1024.in ./input/float1024.in -l -s >> float_results.txt
echo " --------- DONE 1024 ---------"
echo "... script complete."
