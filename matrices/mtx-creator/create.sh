#!/bin/bash
python3 create-mtx.py testfloat4.in "float" 4
python3 create-mtx.py testfloat16.in "float" 16
python3 create-mtx.py testfloat32.in "float" 32
python3 create-mtx.py testfloat64.in "float" 64
python3 create-mtx.py testfloat256.in "float" 256
python3 create-mtx.py testfloat1024.in "float" 1024
python3 create-mtx.py testfloat2048.in "float" 2048
python3 create-mtx.py testfloat8192.in "float" 8192
python3 create-mtx.py testfloat16384.in "float" 16384
