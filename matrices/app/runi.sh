#!/bin/bash
clear
rm -f int_results.txt
echo "---------- START --------"
./mop --mm -f ./input/int64.in ./input/int64.in -l -s >> int_results.txt
echo " --------- DONE 64 ---------"
./mop --mm -f ./input/int128.in ./input/int128.in -l -s >> int_results.txt
echo " --------- DONE 128 --------"
./mop --mm -f ./input/int256.in ./input/int256.in -l -s >> int_results.txt
echo " --------- DONE 256 ---------"
./mop --mm -f ./input/int1024.in ./input/int1024.in -l -s >> int_results.txt
echo " --------- DONE 1024 ---------"
echo "... script complete."
