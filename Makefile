OS=$(shell uname -o)

ifeq ($(OS),Msys)
	OSFLAG=-lmingw32 -lSDL2main
endif

CC=gcc

all: invaders

invaders: main.o ship.o enemy.o listEnemy.o SDL_Label.o
	$(CC) -o invaders main.c ship.c enemy.c listEnemy.c SDL_Label.c $(OSFLAG) -lSDL2 -lSDL2_ttf
	
main.o: main.c main.h
	$(CC) -c main.c $(OSFLAG) -lSDL2
	
SDL_Label.o: SDL_Label.c SDL_Label.h
	$(CC) -c SDL_Label.c -lSDL2 -lSDL2_ttf
	
ship.o: ship.c ship.h
	$(CC) -c ship.c -lSDL2

listEnemy.o: listEnemy.c listEnemy.h
	$(CC) -c listEnemy.c
	
enemy.o: enemy.c enemy.h
	$(CC) -c enemy.c -LSDL2

clean:
	rm *.o
