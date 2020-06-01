#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>

#define SHAPE_SIZE 32
#define NB_SPRITES_MISSILES 30

typedef enum {W, E} Direction;

typedef struct {
	int life;
	int shield;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	SDL_Surface *surface;
	SDL_Texture *texture;
	Direction direction;
	SDL_Surface *surfaceMissile[NB_SPRITES_MISSILES];
	SDL_Texture *textureMissile[NB_SPRITES_MISSILES];
	int nbMissile;
	SDL_bool move;
}Ship;

Ship *new_ship(SDL_Renderer *renderer);
void Ship_update_ship(Ship *ship, SDL_Renderer *renderer);
void Ship_update_missile(Ship *ship, SDL_Renderer *renderer);
void free_ship(Ship *ship);
#endif
