#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

typedef struct {
	int life;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	SDL_Surface *surface;
	SDL_Texture *texture;
}Enemy;

Enemy *new_enemy(SDL_Renderer *renderer);
void free_enemy(Enemy *enemy);
#endif
