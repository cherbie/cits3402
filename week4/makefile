CC=gcc-9 -std=c99 -Wall -Werror -pedantic -fopenmp

all: sequential parallel


sequential: seq_pi.c
	$(CC) -o seq seq_pi.c

parallel: par_pi.c
	$(CC) -o par par_pi.c
