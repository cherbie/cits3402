CC=gcc-9 -std=c99 -fopenmp
WARNINGS=-Wall -Werror -pedantic
OFILES=main.o parser.o helper.o scalar.o operation.o file-reading.o logger.o dealloc.o
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

scalar.o: ./src/scalar.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/scalar.c $(HEADERS)

logger.o: ./src/logger.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/logger.c $(HEADERS)

dealloc.o: ./src/dealloc.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/dealloc.c $(HEADERS)

clean:
	rm -f *.o *.out ./src/*.h.gch
