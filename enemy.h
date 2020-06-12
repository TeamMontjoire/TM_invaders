#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

#define SHAPE_SIZE_ENEMY 32

typedef struct {
	int life;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_bool touch;
	SDL_bool isDestroyed;
	int nbSprite;
}Enemy;

Enemy *new_enemy(SDL_Renderer *renderer);
void Enemy_update_enemy(Enemy *enemy, SDL_Renderer *renderer);
void free_enemy(Enemy *enemy);
#endif
