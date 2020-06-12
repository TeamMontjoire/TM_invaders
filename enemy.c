#include "enemy.h"

Enemy *new_enemy(SDL_Renderer *renderer)
{
	Enemy *this = (Enemy*) malloc(sizeof(Enemy));
	
	this->surface = SDL_LoadBMP("img/enemy.bmp");
	if (this->surface == NULL) {SDL_Log("Couldn't create surface : %s", SDL_GetError());return NULL;}
	SDL_SetColorKey(this->surface, SDL_TRUE, SDL_MapRGB(this->surface->format, 255, 255, 255));
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	if (this->texture == NULL) {SDL_Log("Coulndn't create texture : %s", SDL_GetError());return NULL;}
	
	this->rectSrc.x = 0;
	this->rectSrc.y = 0;
	this->rectSrc.h = SHAPE_SIZE_ENEMY;
	this->rectSrc.w = SHAPE_SIZE_ENEMY;
	this->rectDst.x = 0;
	this->rectDst.y = 0;
	this->rectDst.w = SHAPE_SIZE_ENEMY;
	this->rectDst.h = SHAPE_SIZE_ENEMY;
	this->touch = SDL_FALSE;
	this->isDestroyed = SDL_FALSE;
	this->nbSprite = 0;
	return this;
}

void Enemy_update_enemy(Enemy *this, SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, this->texture, &this->rectSrc, &this->rectDst);
}

void free_enemy(Enemy *this)
{
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	free(this);
}
