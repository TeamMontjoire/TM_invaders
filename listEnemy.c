#include "listEnemy.h"

static int random_between(int min, int max)
{
	return (min + rand()%(max - min + 1));
}

ListEnemy *new_list_enemy(SDL_Renderer *renderer)
{
	ListEnemy *this = (ListEnemy*) malloc(sizeof(ListEnemy));
	
	this->surface = SDL_LoadBMP("img/enemy.bmp");
	if (this->surface == NULL) {SDL_Log("Couldn't create surface : %s", SDL_GetError());return NULL;}
	SDL_SetColorKey(this->surface, SDL_TRUE, SDL_MapRGB(this->surface->format, 255, 255, 255));
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	if (this->texture == NULL) {SDL_Log("Coulndn't create texture : %s", SDL_GetError());return NULL;}
	
	this->surfaceMissile = SDL_LoadBMP("img/missileEnemy.bmp");
	if (this->surfaceMissile == NULL) {SDL_Log("Couldn't create surface missile enemy : %s", SDL_GetError());return NULL;}
	SDL_SetColorKey(this->surfaceMissile, SDL_TRUE, SDL_MapRGB(this->surfaceMissile->format, 255, 255, 255));
	this->textureMissile = SDL_CreateTextureFromSurface(renderer, this->surfaceMissile);
	if (this->textureMissile == NULL) {SDL_Log("Coulndn't create texture missile enemy: %s", SDL_GetError());return NULL;}
	
	for (int i=0;i<NB_ENEMY_MAX;i++) {
		this->enemy[i] = new_enemy(renderer);
	}//end for
	
	for (int i=0; i < NB_SPRITES_MISSILES_ENEMY; i++) {
		this->missile[i].rectSrc.x = 0;
		this->missile[i].rectSrc.y = 0;
		this->missile[i].rectSrc.w = SHAPE_SIZE_ENEMY;
		this->missile[i].rectSrc.h = SHAPE_SIZE_ENEMY;
		this->missile[i].rectDst.x = 0;
		this->missile[i].rectDst.y = 0;
		this->missile[i].rectDst.w = SHAPE_SIZE_ENEMY;
		this->missile[i].rectDst.h = SHAPE_SIZE_ENEMY;
		//~ this->missile[i].wasFired = SDL_FALSE;
		this->missile[i].touchShip = SDL_FALSE;
		this->missile[i].touchMissile = SDL_FALSE;
		this->missile[i].move = SDL_FALSE;
		//~ this->missile[i].isDestroyed = SDL_TRUE;
		//~ this->missile[i].isDestroyed = SDL_FALSE;
		this->missile[i].visible = SDL_FALSE;
		this->missile[i].nbSprite = 0;
	}
	this->nbTotalEnemy = 0;
	this->nbTotalMissile = 0;
	this->nbOfSquad = 0;
	return this;
}

int ListEnemy_new_squad(ListEnemy *this, SDL_Renderer *renderer, int nb_enemy_squad, int pos_squad)
{
	if (this->nbTotalEnemy+nb_enemy_squad > NB_ENEMY_MAX) {
		printf("Error : To much enemy! Enemy must be less than %d.\n", NB_ENEMY_MAX);
		return -1;
	}
	int width, height;
	SDL_GetRendererOutputSize(renderer, &width, &height);
	//~ printf("Taille w=%d\th=%d\n", width, height);
	int width_corridor = (int) width / nb_enemy_squad;
	int pos_corridor = 0;
	for (int i=this->nbTotalEnemy; i < this->nbTotalEnemy+nb_enemy_squad;i++) {
		this->enemy[i]->rectDst.x = random_between(pos_corridor, pos_corridor + width_corridor - SHAPE_SIZE_ENEMY);
		pos_corridor += width_corridor;
		this->enemy[i]->rectDst.y =  - pos_squad * SHAPE_SIZE_ENEMY + 2*SHAPE_SIZE_ENEMY ;//- SHAPE_SIZE_ENEMY;
		this->enemy[i]->visible = SDL_TRUE;
		this->enemy[i]->move = SDL_TRUE;
		//~ printf("Enemy : %d x=%d\n", i, this->enemy[i]->rectDst.x);
	}
	this->nbTotalEnemy += nb_enemy_squad;
	this->nbOfSquad++;
	return 0;
}

void ListEnemy_reset_enemy(ListEnemy *this, int nb_enemy)
{
	Enemy_reset_enemy(this->enemy[nb_enemy]);
}

void ListEnemy_reset_missile(ListEnemy *this, int nb_missile)
{
		this->missile[nb_missile].touchShip = SDL_FALSE;
		this->missile[nb_missile].touchMissile = SDL_FALSE;
		this->missile[nb_missile].move = SDL_FALSE;
		this->missile[nb_missile].visible = SDL_FALSE;
		this->missile[nb_missile].nbSprite = 0;
		this->missile[nb_missile].rectSrc.x = 0;
		this->missile[nb_missile].rectDst.x = 0;
		this->missile[nb_missile].rectDst.y = 0;
}

void ListEnemy_shoot_missile(ListEnemy *this, int nb_missile, int nb_enemy)
{
	this->missile[nb_missile].rectDst.x = this->enemy[nb_enemy]->rectDst.x;
	this->missile[nb_missile].rectDst.y = this->enemy[nb_enemy]->rectDst.y + 16;
	this->missile[nb_missile].move = SDL_TRUE;
	this->missile[nb_missile].visible = SDL_TRUE;
}

void ListEnemy_update_list_enemy(ListEnemy *this, SDL_Renderer *renderer)
{
	for (int i=0; i < this->nbTotalEnemy; i++)
	{
		if (this->enemy[i]->visible)
			Enemy_update_enemy(this->enemy[i], renderer, this->texture);
	}
}

void ListEnemy_update_missile_enemy(ListEnemy *this, SDL_Renderer *renderer)
{
	for (int i=0; i < NB_SPRITES_MISSILES_ENEMY; i++)
	{
		if (this->missile[i].visible)
			SDL_RenderCopy(renderer, this->textureMissile, &this->missile[i].rectSrc, &this->missile[i].rectDst);
	}
}

void free_list_enemy(ListEnemy *this)
{
	for (int i=0;i<NB_ENEMY_MAX;i++) {
		free_enemy(this->enemy[i]);
	}//end for
	SDL_DestroyTexture(this->textureMissile);
	SDL_FreeSurface(this->surfaceMissile);
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	free(this);
}
