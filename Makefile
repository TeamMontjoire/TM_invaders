CC=gcc

all: invaders

invaders: main.o ship.o
	$(CC) -o invaders main.c ship.c -lSDL2
	
main.o: main.c main.h
	$(CC) -c main.c -lSDL2
	
ship.o: ship.c ship.h
	$(CC) -c ship.c -lSDL2
	
clean:
	rm *.o
