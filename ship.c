#include "ship.h"

Ship *new_ship(SDL_Renderer *renderer)
{
	Ship *this = (Ship*) malloc(sizeof(Ship));
	
	this->surface = SDL_LoadBMP("img/ship2.bmp");
	if (this->surface == NULL) {SDL_Log("Couldn't create surface : %s", SDL_GetError());return NULL;}
	SDL_SetColorKey(this->surface, SDL_TRUE, SDL_MapRGB(this->surface->format, 255, 255, 255));
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	if (this->texture == NULL) {SDL_Log("Coulndn't create texture : %s", SDL_GetError());return NULL;}
	
	for (int i=0; i<NB_SPRITES_MISSILES; i++)
	{
		this->missile[i].surface = SDL_LoadBMP("img/missile.bmp");
		if (this->missile[i].surface == NULL) {SDL_Log("Couldn't create surface missile : %s", SDL_GetError());return NULL;}
		SDL_SetColorKey(this->missile[i].surface, SDL_TRUE, SDL_MapRGB(this->missile[i].surface->format, 255, 255, 255));
		this->missile[i].texture = SDL_CreateTextureFromSurface(renderer, this->missile[i].surface);
		if (this->missile[i].texture == NULL) {SDL_Log("Coulndn't create texture missile: %s", SDL_GetError());return NULL;}
		this->missile[i].touch = SDL_FALSE;
		this->missile[i].rectSrc.x = 0;
		this->missile[i].rectSrc.y = 0;
		this->missile[i].rectSrc.w = SHAPE_SIZE;
		this->missile[i].rectSrc.h = SHAPE_SIZE;
		this->missile[i].rectDst.x = 0;
		this->missile[i].rectDst.y = -3*SHAPE_SIZE; // out of screen by default
		this->missile[i].rectDst.w = SHAPE_SIZE;
		this->missile[i].rectDst.h = SHAPE_SIZE;
		this->missile[i].nbSprite = 0;
		//~ this->shoot[i] = SDL_FALSE;
	}
	
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
	
	this->move = SDL_FALSE;
	this->nbMissile = 0;
	return this;
}

void Ship_update_ship(Ship *this, SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, this->texture, &this->rectSrc, &this->rectDst);
}

void Ship_update_missile(Ship *this, SDL_Renderer *renderer)
{
	for (int i=0; i<NB_SPRITES_MISSILES; i++)
	{
			SDL_RenderCopy(renderer, this->missile[i].texture, &this->missile[i].rectSrc, &this->missile[i].rectDst);
	}
}

void free_ship(Ship *this)
{
	for (int i=0; i<NB_SPRITES_MISSILES;i++) {
		SDL_DestroyTexture(this->missile[i].texture);
		SDL_FreeSurface(this->missile[i].surface);
	}
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	free(this);
}
