CC=gcc-9 -std=c99 -fopenmp
WARNINGS=-Wall -Werror -pedantic
OFILES=main.o parser.o helper.o file-reading.o operation.o
HEADERS=./src/mop.h

project: $(OFILES)
	$(CC) $(WARNINGS) -o mop $(OFILES)

main.o: ./src/main.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/main.c $(HEADERS)

parser.o: ./src/parser.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/parser.c $(HEADERS)

helper.o: ./src/helper.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/helper.c $(HEADERS)

file-reading.o: ./src/file-reading.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/file-reading.c $(HEADERS)

operation.o: ./src/operation.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/operation.c $(HEADERS)

clean:
	rm -f *.o *.out
