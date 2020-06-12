#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>

#define SHAPE_SIZE 32
#define NB_SPRITES_MISSILES 30

typedef struct {
	SDL_bool touch;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	SDL_Surface *surface;
	SDL_Texture *texture;
	int nbSprite;
}Missile;

typedef enum {W, E} Direction;

typedef struct {
	int life;
	int shield;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	SDL_Surface *surface;
	SDL_Texture *texture;
	Direction direction;
	Missile missile[NB_SPRITES_MISSILES];
	//~ SDL_Surface *surfaceMissile[NB_SPRITES_MISSILES];
	//~ SDL_Texture *textureMissile[NB_SPRITES_MISSILES];
	int nbMissile;
	SDL_bool move;
	SDL_bool shoot;
}Ship;

Ship *new_ship(SDL_Renderer *renderer);
void Ship_update_ship(Ship *ship, SDL_Renderer *renderer);
void Ship_update_missile(Ship *ship, SDL_Renderer *renderer);
void free_ship(Ship *ship);
#endif
