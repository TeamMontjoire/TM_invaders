#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>

#define SHAPE_SIZE 32
#define SHAPE_SIZE_SHIELD 64
#define NB_SPRITES_MISSILES 80

typedef struct {
	SDL_bool touchEnemy;
	SDL_bool touchMissile;
	//~ SDL_bool isDestroyed;
	SDL_bool visible;
	SDL_bool move;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	int nbSprite; // increment rectDst by SHAPE_SIZE pixel (change sprite)
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
	SDL_Surface *surfaceHalo;
	SDL_Texture *textureHalo;
	SDL_Rect rectSrcHalo;
	SDL_Rect rectDstHalo;
	SDL_Surface *surfaceMissile;
	SDL_Texture *textureMissile;
	int nbMissile;
	SDL_bool move;
	SDL_bool shoot;
}Ship;

Ship *new_ship(SDL_Renderer *renderer);
void Ship_reset_missile(Ship *ship, int nb_missile);
void Ship_update_ship(Ship *ship, SDL_Renderer *renderer);
void Ship_update_missile(Ship *ship, SDL_Renderer *renderer);
void free_ship(Ship *ship);
#endif
