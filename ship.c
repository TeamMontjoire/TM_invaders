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
		
		this->surfaceHalo = SDL_LoadBMP("img/halo.bmp");
		if (this->surfaceHalo == NULL) {SDL_Log("Couldn't create surface halo : %s", SDL_GetError());return NULL;}
		SDL_SetColorKey(this->surfaceHalo, SDL_TRUE, SDL_MapRGBA(this->surfaceHalo->format, 255, 255, 255, 80));
		this->textureHalo = SDL_CreateTextureFromSurface(renderer, this->surfaceHalo);
		if (this->textureHalo == NULL) {SDL_Log("Coulndn't create texture halo: %s", SDL_GetError());return NULL;}
		
	for (int i=0; i<NB_SPRITES_MISSILES; i++)
	{
		this->missile[i].touchEnemy = SDL_FALSE;
		this->missile[i].touchMissile = SDL_FALSE;
		this->missile[i].visible = SDL_FALSE;
		//~ this->missile[i].isDestroyed = SDL_FALSE;
		this->missile[i].move = SDL_FALSE;
		this->missile[i].rectSrc.x = 0;
		this->missile[i].rectSrc.y = 0;
		this->missile[i].rectSrc.w = SHAPE_SIZE;
		this->missile[i].rectSrc.h = SHAPE_SIZE;
		this->missile[i].rectDst.x = -2*SHAPE_SIZE;
		this->missile[i].rectDst.y = -2*SHAPE_SIZE; // out of screen by default
		this->missile[i].rectDst.w = SHAPE_SIZE;
		this->missile[i].rectDst.h = SHAPE_SIZE;
		this->missile[i].nbSprite = 0;
	}
	
	this->life = 3;
	this->shield = 3;
	this->rectSrc.x = 0;
	this->rectSrc.y = 0;
	this->rectSrc.w = SHAPE_SIZE;
	this->rectSrc.h = SHAPE_SIZE;
	this->rectDst.x = 0;
	this->rectDst.y = 0;
	this->rectDst.w = SHAPE_SIZE;
	this->rectDst.h = SHAPE_SIZE;
	
	this->rectSrcHalo.x = 0;
	this->rectSrcHalo.y = 0;
	this->rectSrcHalo.w = SHAPE_SIZE_SHIELD;
	this->rectSrcHalo.h = SHAPE_SIZE_SHIELD;
	this->rectDstHalo.x = 0;
	this->rectDstHalo.y = 0;
	this->rectDstHalo.w = SHAPE_SIZE_SHIELD;
	this->rectDstHalo.h = SHAPE_SIZE_SHIELD;
	
	this->move = SDL_FALSE;
		//~ this->move = SDL_TRUE;

	this->shoot = SDL_FALSE;
	this->nbMissile = 0;
	return this;
}

void Ship_reset_missile(Ship *this, int nb_missile)
{
	this->missile[nb_missile].touchEnemy = SDL_FALSE;
	this->missile[nb_missile].touchMissile = SDL_FALSE;
	this->missile[nb_missile].move = SDL_FALSE;
		//~ this->missile[nb_missile].move = SDL_TRUE;

	this->missile[nb_missile].visible = SDL_FALSE;
	this->missile[nb_missile].nbSprite = 0;
	this->missile[nb_missile].rectSrc.x = 0;
	this->missile[nb_missile].rectSrc.y = 0;
	this->missile[nb_missile].rectDst.x = - 2*SHAPE_SIZE;
	this->missile[nb_missile].rectDst.y = -2*SHAPE_SIZE;
	this->missile[nb_missile].nbSprite = 0;
}

void Ship_update_ship(Ship *this, SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, this->texture, &this->rectSrc, &this->rectDst);
	SDL_RenderCopy(renderer, this->textureHalo, &this->rectSrcHalo, &this->rectDstHalo);
}

void Ship_update_missile(Ship *this, SDL_Renderer *renderer)
{
	for (int i=0; i<NB_SPRITES_MISSILES; i++)
	{
		if (this->missile[i].visible)
			SDL_RenderCopy(renderer, this->textureMissile, &this->missile[i].rectSrc, &this->missile[i].rectDst);
	}
}

void free_ship(Ship *this)
{
	SDL_DestroyTexture(this->textureMissile);
	SDL_FreeSurface(this->surfaceMissile);
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	free(this);
}
