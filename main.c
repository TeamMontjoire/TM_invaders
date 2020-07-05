#include <time.h>

#include "SDL_Label.h"
#include "main.h"
#include "ship.h"
#include "listEnemy.h"

#define SPEED_ENEMY 10 // a great value decreases speed
#define PROBA_SHOOT 5 // GIVE X AND PROBA WILL BE 1/X
#define TIME_SHOOT 500 // a great value decreases time shoot
#define TIME_MOVE_MISSILE 7 // a great value decreases time move missile

void display_rect(SDL_Rect rect)
{
	printf("x=%d \t y=%d\n", rect.x, rect.y);
	printf("h=%d \t w=%d\n", rect.h, rect.w);
}

int random_between(int min, int max)
{
	return (min + rand()%(max - min + 1));
}

SDL_bool screen_limit(SDL_Rect rect) {
	if (rect.x + rect.w > WIDTH) return SDL_TRUE;
	if (rect.x < 0) return SDL_TRUE;
	if (rect.y + rect.h > HEIGHT) return SDL_TRUE;
	if (rect.y < 0) return SDL_TRUE;
	return SDL_FALSE;
}

SDL_bool check_collision(SDL_Rect rect1, SDL_Rect rect2, Margin margin2)
{
	if (rect1.x + rect1.w <= rect2.x) return SDL_FALSE;
	if (rect1.y + rect1.h <= rect2.y) return SDL_FALSE;
	if (rect1.y >= rect2.y + rect2.h - margin2.bottom) return SDL_FALSE;
	if (rect1.x >= rect2.x + rect2.w) return SDL_FALSE;
	return SDL_TRUE;
}

void update_menu(SDL_Label *label, SDL_Renderer *renderer, int score, Ship *ship)
{
	char text_label[80];
	sprintf(text_label, "Score : %d   Shield : %d        Life : %d", score, ship->shield, ship->life);
	SDL_SetTextLabel(label, renderer, text_label);
}

void update_renderer(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 90, 80, 80, 255);
	SDL_RenderClear(renderer);
}

void update_scene(SDL_Renderer *renderer, Ship *ship, ListEnemy *list, SDL_Label *label)
{
	update_renderer(renderer);
	SDL_UpdateLabel(label, renderer);
	ListEnemy_update_list_enemy(list, renderer);
	ListEnemy_update_missile_enemy(list, renderer);
	Ship_update_missile(ship, renderer);
	Ship_update_ship(ship, renderer);
	
	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	SDL_bool quit=SDL_FALSE;
	SDL_bool touchEnemy = SDL_FALSE, touchShip = SDL_FALSE;
	SDL_bool touchMissile = SDL_FALSE, oneShoot = SDL_FALSE;
	Uint32 lastTime=0, currentTime;
	int timeEnemy=0, timeShootMissile=0, timeMoveMissile=0;
	int randEnemy, counterMissileEnemy = 0;
	int score=0;
	//~ char text_label[80];
	Margin marginEnemy, marginMissile;
	
	srand(time(NULL));
	
	if (SDL_VideoInit(NULL) < 0) {SDL_Log("Problem SDL Video init : %s", SDL_GetError());return 1;}
	
	//~ window = SDL_CreateWindow("Invaders",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
		//~ WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
		
	window = SDL_CreateWindow("Invaders",0,0, WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {SDL_Log("Couldn't create renderer : %s", SDL_GetError());return 1;}
	
	Ship *ship = new_ship(renderer);
	ship->rectDst.x = 128;
	ship->rectDst.y = HEIGHT - SHAPE_SIZE;
	ship->rectDstHalo.x = 128-16;
	ship->rectDstHalo.y = HEIGHT - SHAPE_SIZE;
	
	SDL_Label *label = SDL_CreateLabel(renderer, 0, 0, WIDTH, 32, "Score : Shield :  Life :");
	label->colorBackground.r = 120;
	label->colorBackground.g = 110;
	label->colorBackground.b = 110;
	label->colorBackground.a = 230;
	update_menu(label, renderer, 0, ship);
	//~ sprintf(text_label, "Score : %d  Shield : %d", score, ship->shield);
	//~ SDL_SetTextLabel(label, renderer, text_label);	
	
	ListEnemy *list = new_list_enemy(renderer);
	ListEnemy_new_squad(list, renderer, 5, 0);
	ListEnemy_new_squad(list, renderer, 3, 1);
	ListEnemy_new_squad(list, renderer, 6, 2);
	ListEnemy_new_squad(list, renderer, 7, 3);
	marginEnemy.bottom = 16;
	printf("Total enemy : %d\n", list->nbTotalEnemy);
	
	marginMissile.bottom = 0;

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
							if (oneShoot == SDL_FALSE) 
							{
								oneShoot = SDL_TRUE;
								ship->shoot = SDL_TRUE;
							}			
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
						case SDLK_SPACE:
							ship->shoot = SDL_FALSE;
							oneShoot = SDL_FALSE;
							break;
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
			
			/************************************
			*					Move ship
			***********************************/
			if (ship->move) {
				if (ship->direction == W) {
					ship->rectDst.x -= STEP;
					ship->rectDstHalo.x -= STEP;
					if (screen_limit(ship->rectDst)) { ship->rectDst.x +=STEP; ship->rectDstHalo.x +=STEP;}
				}
				if (ship->direction == E) {
					ship->rectDst.x += STEP;
					ship->rectDstHalo.x += STEP;
					if (screen_limit(ship->rectDst)) { ship->rectDst.x -=STEP; ship->rectDstHalo.x -=STEP;}
				}
			}//endif ship move
		
		if (ship->rectSrc.x == 0) ship->rectSrc.x = SHAPE_SIZE; // change sprite ship's player for motor effect
			else ship->rectSrc.x = 0;
		
		/************************************************
		**			Missile Ship player management					*
		*************************************************/
		if (ship->shoot && oneShoot)
		{
			ship->missile[ship->nbMissile].rectDst.x = ship->rectDst.x;
			ship->missile[ship->nbMissile].rectDst.y = HEIGHT - 2*SHAPE_SIZE;
			ship->missile[ship->nbMissile].visible = SDL_TRUE;
			ship->missile[ship->nbMissile].move = SDL_TRUE;
			ship->nbMissile++;
			//~ printf("Missile num %d\n", ship->nbMissile);
			if (ship->nbMissile == NB_SPRITES_MISSILES) ship->nbMissile = 0; // re init nb missile
			ship->shoot = SDL_FALSE;
		}//endif shoot
		
		/** if missile is inside screen and don't touch enemy or missile then he's keep going up **/
		for (int i=0;i<NB_SPRITES_MISSILES;i++)
		{	
			//~ if (ship->missile[i].rectDst.y > -SHAPE_SIZE && ship->missile[i].touchEnemy == SDL_FALSE && ship->missile[i].touchMissile == SDL_FALSE)
			if (ship->missile[i].move == SDL_TRUE && ship->missile[i].touchEnemy == SDL_FALSE && ship->missile[i].touchMissile == SDL_FALSE)
			{
				ship->missile[i].rectDst.y -= STEP;
			}
			
			if (ship->missile[i].rectDst.y < -SHAPE_SIZE) Ship_reset_missile(ship, i);
			
			/** Destruct missile ship **/
			if (ship->missile[i].touchEnemy || ship->missile[i].touchMissile) {
				printf("Destruct missile ship\n");
				switch (ship->missile[i].nbSprite) {
					case 0:
						ship->missile[i].rectSrc.x = SHAPE_SIZE_ENEMY;
						ship->missile[i].nbSprite++;
						break;
					case 1:
						ship->missile[i].rectSrc.x = 2*SHAPE_SIZE_ENEMY;
						ship->missile[i].nbSprite++;
						break;
					case 2:
						Ship_reset_missile(ship, i);
						break;
				}//endswitch
			}//endif destruct missile
		}//endfor missile ship
		
		
			/******************************
			*		Enemy management 
			*******************************/
			/** Move Enemy **/
			if (timeEnemy >= SPEED_ENEMY*DELTA_TIME) {
				for (int j=0;j<list->nbTotalEnemy;j++) {
					if (list->enemy[j]->move)
						list->enemy[j]->rectDst.y += SHAPE_SIZE_ENEMY/4; //else enemy continue to keep going down
					//~ timeEnemy = 0;
				}//endfor
				timeEnemy = 0;
			} else {
				timeEnemy += DELTA_TIME;
			}//endif timeShootEnemy
			
			/** Touch and Destruct Enemy **/
			for (int i=0; i < list->nbTotalEnemy; i++)
			{
				for (int j=0; j < ship->nbMissile; j++) {
					touchEnemy = check_collision(ship->missile[j].rectDst, list->enemy[i]->rectDst, marginEnemy);
					if (touchEnemy) {
						list->enemy[i]->isTouched = SDL_TRUE;
						list->enemy[i]->move = SDL_FALSE;
						ship->missile[j].touchEnemy = SDL_TRUE;
						//~ ship->missile[j].visible = SDL_FALSE;
						ship->missile[j].move = SDL_FALSE;
						printf("Enemy num %d touch\n", i);
					}
				}//end for missile ship
				
				/** Destruct enemy **/
				if (list->enemy[i]->isTouched) {
					switch (list->enemy[i]->nbSprite) {
						case 0:
							list->enemy[i]->rectSrc.x = 2*SHAPE_SIZE_ENEMY;
							list->enemy[i]->nbSprite++;
							break;
						case 1:
							list->enemy[i]->rectSrc.x = 3*SHAPE_SIZE_ENEMY;
							list->enemy[i]->nbSprite++;
							break;
						case 2:
							list->enemy[i]->rectSrc.x = 4*SHAPE_SIZE_ENEMY;
							list->enemy[i]->nbSprite++;
							break;
						case 3:
							list->enemy[i]->rectSrc.x = 5*SHAPE_SIZE_ENEMY;
							list->enemy[i]->nbSprite++;
							break;
						case 4:
							score++;
							update_menu(label, renderer, score, ship);
							ListEnemy_reset_enemy(list, i);
							break;
					}//endswitch destruct
				}//endif destruct
				
				 /** Change sprite enemy while the enemys moving **/
				 if (list->enemy[i]->isTouched == SDL_FALSE)
				 {
					 if (list->enemy[i]->rectSrc. x == 0)
					 {
						 list->enemy[i]->rectSrc.x = SHAPE_SIZE_ENEMY;
					 }
						else list->enemy[i]->rectSrc.x = 0;
					}//endif is no touche
			 
			}//end for touch enemy
			
			
			/************************************
			 * 	Manage Missile Enemy
			 ***********************************/
			 /** Move missile enemy **/
			 if (timeMoveMissile >= TIME_MOVE_MISSILE * DELTA_TIME) {
				 for (int i=0; i < NB_SPRITES_MISSILES_ENEMY; i++) {
					 if (list->missile[i].move) {
						//~ printf("Move missile %d\n", i);
						list->missile[i].rectDst.y += SHAPE_SIZE_ENEMY;
					}
					/** if missile comes down then reinit **/
					if (list->missile[i].rectDst.y >= HEIGHT) ListEnemy_reset_missile(list, i);
				 }
				 timeMoveMissile = 0;
			 } else {
				 timeMoveMissile += DELTA_TIME;
			 }//endif timeMoveMissile
			 
			 /** Shoot of missile enemy **/ 
				for (int i=0; i < NB_SPRITES_MISSILES_ENEMY; i++)
				{						
					
					if (timeShootMissile >= TIME_SHOOT * DELTA_TIME) {
					 if (list->missile[i].visible == SDL_FALSE) {
						 randEnemy = random_between(0, list->nbTotalEnemy);
						 ListEnemy_shoot_missile(list, i, randEnemy);
					 }//endif not visible
					 timeShootMissile = 0;
					} else {
						timeShootMissile += DELTA_TIME;
					}
					 
					for (int j=0; j < NB_SPRITES_MISSILES; j++)
					{
						touchMissile = check_collision(ship->missile[j].rectDst, list->missile[i].rectDst, marginEnemy);
						//~ touchShip = check_collision(ship->rectDst, list->missile[i].rectDst, marginMissile);

						//~ printf("Collision missile missile enemy %d avec missile ship %d\n", i, j);
						if (touchMissile) {
								printf("Collision missile missile enemy %d avec missile ship %d\n", i, j);
								ship->missile[j].move = SDL_FALSE;
								ship->missile[j].touchMissile = SDL_TRUE;
								list->missile[i].move = SDL_FALSE;
							//~ SDL_Delay(500);
							switch (list->missile[i].nbSprite) {
								case 0:
									list->missile[i].rectSrc.x = SHAPE_SIZE_ENEMY;
									list->missile[i].nbSprite++;
									printf("Sprite %d\t rectSrc.x = %d\t visible = %d\n", list->missile[i].nbSprite, list->missile[i].rectSrc.x, list->missile[i].visible);
									break;
								case 1:
									list->missile[i].rectSrc.x = 2*SHAPE_SIZE_ENEMY;
									list->missile[i].nbSprite++;
									printf("Sprite %d\t rectSrc.x = %d\n", list->missile[i].nbSprite, list->missile[i].rectSrc.x);
									break;
								case 2:
									Ship_reset_missile(ship, j);
									ListEnemy_reset_missile(list, i);
									printf("Re init missile enemy\n");
									break;
							}//endswitch nb sprite
						}//endif touch missile
					}//endfor nb sprites missiles ship
					
					touchShip = check_collision(ship->rectDst, list->missile[i].rectDst, marginMissile);
					if (touchShip) 
					{
						printf("Collision ship avec missile enemy %d\n", i);
						//~ list->missile[i].touchShip = SDL_TRUE;
						ship->shield--;
						if (ship->shield == 0)
						{
							ship->shield = 3;
							ship->life--;
						}
						update_menu(label, renderer, score, ship);
						list->missile[i].move = SDL_FALSE;
							//~ SDL_Delay(500);
							switch (list->missile[i].nbSprite) {
								case 0:
									list->missile[i].rectSrc.x = SHAPE_SIZE_ENEMY;
									list->missile[i].nbSprite++;
									printf("Sprite %d\t rectSrc.x = %d\t visible = %d\n", list->missile[i].nbSprite, list->missile[i].rectSrc.x, list->missile[i].visible);
									break;
								case 1:
									list->missile[i].rectSrc.x = 2*SHAPE_SIZE_ENEMY;
									list->missile[i].nbSprite++;
									printf("Sprite %d\t rectSrc.x = %d\n", list->missile[i].nbSprite, list->missile[i].rectSrc.x);
									break;
								case 2:
									ListEnemy_reset_missile(list, i);
									printf("Re init missile enemy\n");
									break;
							}//endswitch nb sprite
					}//end touch ship
				}//endfor nb sprites missiles enemy
		
			/**********************
			 * END GAME PROCESSING
			 ***********************/
		
			update_scene(renderer, ship, list, label);
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
