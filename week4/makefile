CC=gcc-9 -std=c99 -Wall -Werror -pedantic -fopenmp

all: sequential parallel wrong


sequential: seq_pi.c
	$(CC) -o seq seq_pi.c

parallel: par_pi.c
	$(CC) -o par par_pi.c

wrong: wrong.c
	$(CC) -o wrong wrong.c
