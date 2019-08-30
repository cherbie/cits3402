CC=gcc-9 -std=c99 -Wall -Werror -pedantic -fopenmp

all: parallel matrices

parallel: pi-calc.c
	$(CC) -o pi pi-calc.c

matrices: matrixmul.c
	$(CC) -o matrix matrixmul.c