#ifndef LIST_ENMEY_H
#define LIST_ENEMY_H

#include "enemy.h"

#define NB_ENEMY_MAX 30
#define NB_SPRITES_MISSILES_ENEMY 80

typedef struct {
	SDL_bool touchShip;
	SDL_bool touchMissile;
	SDL_bool visible;
	//~ SDL_bool isDestroyed;
	SDL_bool move;
	SDL_Rect rectSrc;
	SDL_Rect rectDst;
	int nbSprite; // increment rectDst by SHAPE_SIZE_ENEMY pixel (change sprite)
}MissileEnemy;

typedef struct {
	Enemy *enemy[NB_ENEMY_MAX];
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Surface *surfaceMissile;
	SDL_Texture *textureMissile;
	MissileEnemy missile[NB_SPRITES_MISSILES_ENEMY];
	int nbTotalEnemy;
	int nbTotalMissile;
	int nbOfSquad;
}ListEnemy;

ListEnemy *new_list_enemy(SDL_Renderer *renderer);
int ListEnemy_new_squad(ListEnemy *listEnemy, SDL_Renderer *renderer, int nb_enemy, int pos_squad);
void ListEnemy_reset_enemy(ListEnemy *listEnemy, int nb_enemy);
void ListEnemy_reset_missile(ListEnemy *listEnemy, int nb_missile);
void ListEnemy_shoot_missile(ListEnemy *listEnemy, int nb_missile, int nb_enemy);
void ListEnemy_update_list_enemy(ListEnemy *listEnemy, SDL_Renderer *renderer);
void ListEnemy_update_missile_enemy(ListEnemy *listEnemy, SDL_Renderer *renderer);
void free_list_enemy(ListEnemy *listEnemy);
#endif
