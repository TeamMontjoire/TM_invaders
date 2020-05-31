#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>

#define SHAPE_SIZE 32

typedef struct {
	int life;
	int shield;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Surface *surfaceMissile;
	SDL_Texture *textureMissile;
	SDL_bool shoot;
}Ship;

Ship *new_ship(SDL_Renderer *renderer);
void Ship_update_ship(Ship *ship, SDL_Renderer *renderer);
void Ship_update_missile(Ship *ship, SDL_Renderer *renderer);
void free_ship(Ship *ship);
#endif
