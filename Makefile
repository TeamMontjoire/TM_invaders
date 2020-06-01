OS=$(shell uname -o)

ifeq ($(OS),Msys)
	OSFLAG=-lmingw32 -lSDL2main
endif

CC=gcc

all: invaders

invaders: main.o ship.o enemy.o
	$(CC) -o invaders main.c ship.c enemy.c $(OSFLAG) -lSDL2
	
main.o: main.c main.h
	$(CC) -c main.c $(OSFLAG) -lSDL2
	
ship.o: ship.c ship.h
	$(CC) -c ship.c -lSDL2
	
enemy.o: enemy.c enemy.h
	$(CC) -c enemy.c -LSDL2

clean:
	rm *.o
