#include "listEnemy.h"

ListEnemy *new_list_enemy(SDL_Renderer *renderer)
{
	ListEnemy *this = (ListEnemy*) malloc(sizeof(ListEnemy));
	for (int i=0;i<NB_ENEMY;i++) {
		this->enemy[i] = new_enemy(renderer);
	}//end for
	return this;
}

void free_list_enemy(ListEnemy *this)
{
	for (int i=0;i<NB_ENEMY;i++) {
		free_enemy(this->enemy[i]);
	}//end for
	free(this);
}
