CC=gcc-9 -std=c99 -fopenmp
WARNINGS=-Wall -Werror -pedantic
OFILES=main.o parser.o helper.o
HEADERS=mop.h

project: $(OFILES)
	$(CC) $(WARNINGS) -o mop $(OFILES)

main.o: main.c $(HEADERS)
	$(CC) $(WARNINGS) -c main.c $(HEADERS)

parser.o: parser.c $(HEADERS)
	$(CC) $(WARNINGS) -c parser.c $(HEADERS)

helper.o: helper.c $(HEADERS)
	$(CC) $(WARNINGS) -c helper.c $(HEADERS)

clean:
	rm -f *.o *.out
