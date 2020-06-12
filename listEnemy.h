#ifndef LIST_ENMEY_H
#define LIST_ENEMY_H

#include "enemy.h"

#define NB_ENEMY 10

typedef struct {
	Enemy *enemy[NB_ENEMY];
	
}ListEnemy;

ListEnemy *new_list_enemy(SDL_Renderer *renderer);
void free_list_enemy(ListEnemy *listEnemy);
#endif
