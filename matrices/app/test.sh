#!/bin/bash
clear
echo "start"
./mop --ts -f ./input/file3.in ./input/file4.in
echo "next"
./mop --ts -f ./input/file3.in ./input/file4.in -s
