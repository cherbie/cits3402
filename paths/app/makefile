# clang wrapper
CC=mpicc
WARNINGS=-Wall -Werror -pedantic
OBJECT=main.o
HEADERS=./src/sp.h

sp: $(OBJECT)
	$(CC) $(WARNINGS) -o sp $(OBJECT)

main.o: ./src/main.c $(HEADERS)
	$(CC) $(WARNINGS) -c ./src/main.c

clean:
	rm -f *.o
