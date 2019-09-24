CC=gcc-9 -std=c99 -fopenmp
WARNINGS=-Wall -Werror -pedantic
OFILES=main.o parser.o helper.o sync-coo.o operation.o  sync-csr.o file-reading.o logger.o dealloc.o async-coo.o async-csr.o #async-filereading.o
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

sync-coo.o: ./src/sync-coo.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/sync-coo.c $(HEADERS)

sync-csr.o: ./src/sync-csr.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/sync-csr.c $(HEADERS)

logger.o: ./src/logger.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/logger.c $(HEADERS)

dealloc.o: ./src/dealloc.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/dealloc.c $(HEADERS)

async-coo.o: ./src/async-coo.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/async-coo.c $(HEADERS)

async-csr.o: ./src/async-csr.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/async-csr.c $(HEADERS)

async-filereading.o: ./src/async-filereading.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/async-filereading.c $(HEADERS)

clean:
	rm -f *.o *.out ./src/*.h.gch ./src/*.o

clean-all:
	rm -f *.o *.out *.txt ./src/*.h.gch
