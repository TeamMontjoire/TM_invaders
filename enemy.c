#include "enemy.h"

Enemy *new_enemy(SDL_Renderer *renderer)
{
	Enemy *this = (Enemy*) malloc(sizeof(Enemy));
			
	this->rectSrc.x = 0;
	this->rectSrc.y = 0;
	this->rectSrc.h = SHAPE_SIZE_ENEMY;
	this->rectSrc.w = SHAPE_SIZE_ENEMY;
	this->rectDst.x = - 2 * SHAPE_SIZE_ENEMY; // out of screen by default
	this->rectDst.y = 0;
	this->rectDst.w = SHAPE_SIZE_ENEMY;
	this->rectDst.h = SHAPE_SIZE_ENEMY;
	this->isTouched = SDL_FALSE;
	//~ this->isDestroyed = SDL_FALSE;
	this->visible = SDL_FALSE;
	this->move = SDL_FALSE;
	this->nbSprite = 0;
	//~ this->nbLastMissile = 0;
	return this;
}

void Enemy_reset_enemy(Enemy *this)
{
	this->rectSrc.x = 0;
	this->rectSrc.y = 0;
	this->rectDst.x = -2 * SHAPE_SIZE_ENEMY;
	this->rectDst.y = 0;
	this->isTouched = SDL_FALSE;
	this->visible = SDL_FALSE;
	this->move = SDL_FALSE;
	this->nbSprite = 0;
}

void Enemy_update_enemy(Enemy *this, SDL_Renderer *renderer, SDL_Texture *texture)
{
	if (this->visible)
		SDL_RenderCopy(renderer, texture, &this->rectSrc, &this->rectDst);
}

void free_enemy(Enemy *this)
{
	free(this);
}
