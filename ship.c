#include "ship.h"

Ship *new_ship(SDL_Renderer *renderer)
{
	Ship *this = (Ship*) malloc(sizeof(Ship));
	
	this->surface = SDL_LoadBMP("img/ship2.bmp");
	if (this->surface == NULL) {SDL_Log("Couldn't create surface : %s", SDL_GetError());return NULL;}
	SDL_SetColorKey(this->surface, SDL_TRUE, SDL_MapRGB(this->surface->format, 255, 255, 255));
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	if (this->texture == NULL) {SDL_Log("Coulndn't create texture : %s", SDL_GetError());return NULL;}
	
	this->surfaceMissile = SDL_LoadBMP("img/missile.bmp");
	if (this->surfaceMissile == NULL) {SDL_Log("Couldn't create surface missile : %s", SDL_GetError());return NULL;}
	SDL_SetColorKey(this->surfaceMissile, SDL_TRUE, SDL_MapRGB(this->surfaceMissile->format, 255, 255, 255));
	this->textureMissile = SDL_CreateTextureFromSurface(renderer, this->surfaceMissile);
	if (this->textureMissile == NULL) {SDL_Log("Coulndn't create texture missile: %s", SDL_GetError());return NULL;}
	
	this->life = 3;
	this->shield = 10;
	this->rectSrc.x = 0;
	this->rectSrc.y = 0;
	this->rectSrc.w = SHAPE_SIZE;
	this->rectSrc.h = SHAPE_SIZE;
	this->rectDst.x = 0;
	this->rectDst.y = 0;
	this->rectDst.w = SHAPE_SIZE;
	this->rectDst.h = SHAPE_SIZE;
	
	this->shoot = SDL_FALSE;
	return this;
}

void Ship_update_ship(Ship *this, SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, this->texture, &this->rectSrc, &this->rectDst);
}

void Ship_update_missile(Ship *this, SDL_Renderer *renderer)
{
	if (this->shoot)
		SDL_RenderCopy(renderer, this->textureMissile, NULL, &this->surfaceMissile->clip_rect);
}

void free_ship(Ship *this)
{
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	SDL_FreeSurface(this->surfaceMissile);
	free(this);
}
