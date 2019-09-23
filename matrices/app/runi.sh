#!/bin/bash
clear
rm -f int_results.txt
echo "---------- START --------"
./mop --sc 2 -f ./input/int64.in ./input/int64.in -l -s >> int_results.txt
echo " --------- DONE 64 ---------"
./mop --sc 4 -f ./input/int128.in ./input/int128.in -l -s >> int_results.txt
echo " --------- DONE 128 --------"
./mop --sc 3 -f ./input/int256.in ./input/int256.in -l -s >> int_results.txt
echo " --------- DONE 256 ---------"
./mop --sc 9 -f ./input/int1024.in ./input/int1024.in -l -s >> int_results.txt
echo " --------- DONE 1024 ---------"
echo "... script complete."
