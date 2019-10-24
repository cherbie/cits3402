# clang wrapper
CC=mpicc
WARNINGS=-Wall -Werror -pedantic
OBJECT=main.o filereading.o parser.o dealloc.o errors.o helpers.o shortestpath.o
HEADERS=./src/sp.h

sp: $(OBJECT)
	$(CC) $(WARNINGS) -o sp $(OBJECT)

main.o: ./src/main.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/main.c

filereading.o: ./src/filereading.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/filereading.c

parser.o: ./src/parser.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/parser.c

dealloc.o: ./src/dealloc.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/dealloc.c

errors.o: ./src/errors.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/errors.c

helpers.o: ./src/helpers.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/helpers.c

shortestpath.o: ./src/shortestpath.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/shortestpath.c

clean:
	rm -f *.o *.out

clean-all:
	rm -f *.o *.out ./results/* ./time/*
