#!/bin/bash
clear
./mop --mm -f ./test/testfloat32.in ./test/testfloat32.in -s >> result_float_mm.txt
./mop --mm -f ./test/testfloat32.in ./test/testfloat32.in >> result_float_mm.txt
./mop --mm -f ./test/testfloat64.in ./test/testfloat64.in -s >> result_float_mm.txt
./mop --mm -f ./test/testfloat64.in ./test/testfloat64.in >> result_float_mm.txt
./mop --mm -f ./test/testfloat256.in ./test/testfloat256.in -s >> result_float_mm.txt
./mop --mm -f ./test/testfloat256.in ./test/testfloat256.in >> result_float_mm.txt
echo "reached the bignum"
./mop --mm -f ./test/testfloat1024.in ./test/testfloat1024.in -s >> result_float_mm.txt
./mop --mm -f ./test/testfloat1024.in ./test/testfloat1024.in >> result_float_mm.txt
echo "last stfloat"
./mop --mm -f ./test/testfloat2048.in ./test/testfloat2048.in -s >> result_float_mm.txt
./mop --mm -f ./test/testfloat2048.in ./test/testfloat2048.in >> result_float_mm.txt
#./mop --mm -f ./test/testfloat8192.in ./test/testfloat8192.in -s >> result_float_mm.txt
#./mop --mm -f ./test/testfloat8192.in ./test/testfloat8192.in >> result_float_mm.txt
#./mop --mm -f ./test/testfloat16384.in ./test/testfloat16384.in -s >> result_float_mm.txt
#./mop --mm -f ./test/testfloat16384.in ./test/testfloat16384.in >> result_float_mm.txt
echo "... script complete"
