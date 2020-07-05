#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

#define SHAPE_SIZE_ENEMY 32

typedef struct {
	int life;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	//~ SDL_bool isDestroyed;
	SDL_bool isTouched;
	SDL_bool visible;
	SDL_bool move;
	//~ MissileEnemy missile[NB_SPRITES_MISSILES_ENEMY];
	int nbSprite;
	//~ int nbLastMissile;
}Enemy;

Enemy *new_enemy(SDL_Renderer *renderer);
void Enemy_reset_enemy(Enemy *enemy);
void Enemy_update_enemy(Enemy *enemy, SDL_Renderer *renderer, SDL_Texture *texture);
//~ void Enemy_update_missile(Enemy *enemy, SDL_Renderer *renderer, SDL_Texture *texture);
void free_enemy(Enemy *enemy);
#endif
