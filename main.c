#include <time.h>

#include "main.h"
#include "ship.h"
#include "listEnemy.h"
#include "enemy.h"

void display_rect(SDL_Rect rect)
{
	printf("x=%d \t y=%d\n", rect.x, rect.y);
	printf("h=%d \t w=%d\n", rect.h, rect.w);
}

int random_between(int min, int max)
{
	//~ return SHAPE_SIZE * (min + rand()%(max - min + 1));
		return (min + rand()%(max - min + 1));
}

SDL_bool screen_limit(SDL_Rect rect) {
	if (rect.x + rect.w > WIDTH) return SDL_TRUE;
	if (rect.x < 0) return SDL_TRUE;
	if (rect.y + rect.h > HEIGHT) return SDL_TRUE;
	if (rect.y < 0) return SDL_TRUE;
	return SDL_FALSE;
}

SDL_bool check_collision(SDL_Rect rect1, SDL_Rect rect2, Margin margin)
{
	if (rect1.x + rect1.w <= rect2.x) return SDL_FALSE;
	if (rect1.y + rect1.h <= rect2.y) return SDL_FALSE;
	if (rect1.y >= rect2.y + rect2.h - margin.bottom) return SDL_FALSE;
	if (rect1.x >= rect2.x + rect2.w) return SDL_FALSE;
	return SDL_TRUE;
}

void update_renderer(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 90, 80, 80, 255);
	SDL_RenderClear(renderer);
}

//~ void update_scene(SDL_Renderer *renderer, Ship *ship, Enemy *enemy)
//~ {
	//~ update_renderer(renderer);
	//~ if (!enemy->isDestroyed) Enemy_update_enemy(enemy, renderer);
	//~ Ship_update_missile(ship, renderer);
	//~ Ship_update_ship(ship, renderer);
	//~ SDL_RenderPresent(renderer);
//~ }

void update_scene(SDL_Renderer *renderer, Ship *ship, ListEnemy *list)
{
	update_renderer(renderer);
	for (int i=0;i<NB_ENEMY;i++) {
		if (!list->enemy[i]->isDestroyed) Enemy_update_enemy(list->enemy[i], renderer);
	}
	//~ if (!enemy->isDestroyed) Enemy_update_enemy(enemy, renderer);
	Ship_update_missile(ship, renderer);
	Ship_update_ship(ship, renderer);
	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	SDL_bool quit=SDL_FALSE, touchEnemy = SDL_FALSE;
	Uint32 lastTime=0, currentTime;
	int timeEnemy=0;
	Margin marginEnemy;
	
	srand(time(NULL));
	int COL = WIDTH - SHAPE_SIZE_ENEMY;
	//~ int COL = WIDTH - SHAPE_SIZE_ENEMY;
	
	if (SDL_VideoInit(NULL) < 0) {SDL_Log("Problem SDL Video init : %s", SDL_GetError());return 1;}
	
	//~ window = SDL_CreateWindow("Invaders",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
		//~ WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
		
		window = SDL_CreateWindow("Invaders",0,0,
		WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {SDL_Log("Couldn't create renderer : %s", SDL_GetError());return 1;}
	
	Ship *ship = new_ship(renderer);
	ship->rectDst.x = 128;

	ship->rectDst.y = HEIGHT - SHAPE_SIZE;
	
	ListEnemy *list = new_list_enemy(renderer);
	int WIDTH_CORRIDOR = WIDTH / NB_ENEMY;
	int POS_CORRIDOR =0;
	for (int i=0;i<NB_ENEMY;i++) {
		list->enemy[i]->rectDst.x = random_between(POS_CORRIDOR, POS_CORRIDOR+WIDTH_CORRIDOR-SHAPE_SIZE_ENEMY);
		POS_CORRIDOR += WIDTH_CORRIDOR;
		//~ printf("pos x : %d\n", list->enemy[i]->rectDst.x);
	}
	marginEnemy.bottom = 16;

	while (!quit) {
		if ( SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					quit=SDL_TRUE;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							quit=SDL_TRUE;
							break;
						case SDLK_SPACE:
							ship->missile[ship->nbMissile].rectDst.x = ship->rectDst.x;
							ship->missile[ship->nbMissile].rectDst.y = HEIGHT - 2*SHAPE_SIZE;
							ship->nbMissile++;
							if (ship->nbMissile == NB_SPRITES_MISSILES) ship->nbMissile = 0;
							break;
						case SDLK_LEFT:
							ship->direction = W;
							ship->move = SDL_TRUE;
							break;
						case SDLK_RIGHT:
							ship->direction = E;
							ship->move = SDL_TRUE;
							break;
					}//end switch event.key.keysym.sym keydown
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							ship->move = SDL_FALSE;
							break;
						case SDLK_RIGHT:
							ship->move = SDL_FALSE;
							break;
					}//end switch event.key.keysym.sym keyup
					break;
			}//end switch event.type
		}//endif PollEvent
		
		/******************************************
		 * GAME PROCESSING
		 *****************************************/
		currentTime = SDL_GetTicks();
		if (currentTime - lastTime > DELTA_TIME) {
			
			/* Move ship */
			if (ship->move) {
				if (ship->direction == W) {
					ship->rectDst.x -= STEP;
					if (screen_limit(ship->rectDst)) ship->rectDst.x +=STEP;
				}
				if (ship->direction == E) {
					ship->rectDst.x += STEP;
					if (screen_limit(ship->rectDst)) ship->rectDst.x -=STEP;
				}
			}//endif ship move
		
		/* Missile management */
		for (int i=0;i<NB_SPRITES_MISSILES;i++)
		{	
			for (int j=0;j<NB_ENEMY;j++) {
				touchEnemy = check_collision(ship->missile[i].rectDst, list->enemy[j]->rectDst, marginEnemy);
				if (touchEnemy) list->enemy[j]->touch = ship->missile[i].touch = SDL_TRUE;
			}//end for NB_ENEMY
			
			/* if missile is inside screen then he's keep going up */
			if (ship->missile[i].rectDst.y > -SHAPE_SIZE && ship->missile[i].touch == SDL_FALSE) ship->missile[i].rectDst.y -= STEP;
			 
			if (ship->missile[i].touch) {
				switch (ship->missile[i].nbSprite) {
					case 0:
						ship->missile[i].rectSrc.x = 2*SHAPE_SIZE;
						ship->missile[i].nbSprite++;
						break;
					case 1:
						ship->missile[i].rectSrc.x = 3*SHAPE_SIZE;
						ship->missile[i].nbSprite++;
						break;
					case 2:
						ship->missile[i].rectSrc.x = 4*SHAPE_SIZE;
						ship->missile[i].nbSprite++;
						break;
				}//end switch nb sprite
			}//endif missile touch
		}//endfor NB_SPRITES_MISSILES
			
			if (ship->rectSrc.x == 0) ship->rectSrc.x = SHAPE_SIZE; // change sprite ship's player
				else ship->rectSrc.x = 0;
			
			for (int j=0;j<NB_ENEMY;j++) {
				if (list->enemy[j]->rectSrc.x == 0) list->enemy[j]->rectSrc.x = SHAPE_SIZE_ENEMY; //change sprite enemy
						else list->enemy[j]->rectSrc.x = 0;
			}
			
			/* Enemy management */
			//~ if (timeEnemy >= 6 * DELTA_TIME) {
				for (int j=0;j<NB_ENEMY;j++) {
					if (list->enemy[j]->touch) {
						printf("nbSprite %d\n", list->enemy[j]->nbSprite);
						switch (list->enemy[j]->nbSprite) {
							case 0:
								list->enemy[j]->rectSrc.x = 2*SHAPE_SIZE_ENEMY;
								list->enemy[j]->nbSprite++;
								break;
							case 1:
								list->enemy[j]->rectSrc.x = 3*SHAPE_SIZE_ENEMY;
								list->enemy[j]->nbSprite++;
								break;
							case 2:
								list->enemy[j]->rectSrc.x = 4*SHAPE_SIZE_ENEMY;
								list->enemy[j]->nbSprite++;
								break;
							case 3:
								list->enemy[j]->rectSrc.x = 5*SHAPE_SIZE_ENEMY;
								list->enemy[j]->nbSprite++;
								break;
							case 4:
								list->enemy[j]->isDestroyed = SDL_TRUE;
								/* put enemy out of screen */
								list->enemy[j]->rectDst.x = -SHAPE_SIZE_ENEMY;
								list->enemy[j]->rectDst.y = -SHAPE_SIZE_ENEMY;
								break;
						}//end switch nbSprite
					} else {
						if (timeEnemy >= 7 * DELTA_TIME) {
							list->enemy[j]->rectDst.y += SHAPE_SIZE_ENEMY/4; //else enemy continue to keep going down
							timeEnemy = 0;
						} else {
							timeEnemy += DELTA_TIME;
						}
					}//endif enemy touch
				}//endfor
			//~ } else {
				//~ timeEnemy += DELTA_TIME;
			//~ }
			/**********************
			 * END GAME PROCESSING
			 * *********************/
		
			update_scene(renderer, ship, list);
			lastTime = currentTime;
		}//endif DELTA_TIME		
	}//end while
	
	free_list_enemy(list);
	free_ship(ship);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
