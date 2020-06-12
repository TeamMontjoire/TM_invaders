OS=$(shell uname -o)

ifeq ($(OS),Msys)
	OSFLAG=-lmingw32 -lSDL2main
endif

CC=gcc

all: invaders

invaders: main.o ship.o enemy.o listEnemy.o
	$(CC) -o invaders main.c ship.c enemy.c listEnemy.c $(OSFLAG) -lSDL2
	
main.o: main.c main.h
	$(CC) -c main.c $(OSFLAG) -lSDL2
	
ship.o: ship.c ship.h
	$(CC) -c ship.c -lSDL2

listEnemy.o: listEnemy.c listEnemy.h
	$(CC) -c listEnemy.c
	
enemy.o: enemy.c enemy.h
	$(CC) -c enemy.c -LSDL2

clean:
	rm *.o
