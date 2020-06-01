#include "main.h"
#include "ship.h"
#include "enemy.h"

SDL_bool screen_limit(SDL_Rect rect) {
	if (rect.x + rect.w > WIDTH) return SDL_TRUE;
	if (rect.x < 0) return SDL_TRUE;
	if (rect.y + rect.h > HEIGHT) return SDL_TRUE;
	if (rect.y < 0) return SDL_TRUE;
	return SDL_FALSE;
}

void update_renderer(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 90, 80, 80, 255);
	SDL_RenderClear(renderer);
}

void update_scene(SDL_Renderer *renderer, Ship *ship)
{
	update_renderer(renderer);
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
	Uint32 lastTime=0, currentTime;
	
	if (SDL_VideoInit(NULL) < 0) {SDL_Log("Problem SDL Video init : %s", SDL_GetError());return 1;}
	
	window = SDL_CreateWindow("Invaders",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
		WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
		
		//~ window = SDL_CreateWindow("Invaders",0,0,
		//~ WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {SDL_Log("Couldn't create renderer : %s", SDL_GetError());return 1;}
	
	Ship *ship = new_ship(renderer);
	//~ ship->rectDst.x = ship->surfaceMissile->clip_rect.x = 128;
		ship->rectDst.x = 128;

	ship->rectDst.y = HEIGHT - SHAPE_SIZE;
	update_scene(renderer, ship);

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
							ship->nbMissile++;
							ship->surfaceMissile[ship->nbMissile]->clip_rect.x = ship->rectDst.x;
							ship->surfaceMissile[ship->nbMissile]->clip_rect.y = HEIGHT - 2*SHAPE_SIZE;
							if (ship->nbMissile == NB_SPRITES_MISSILES-1) ship->nbMissile = 0;
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
		
		currentTime = SDL_GetTicks();
		if (currentTime - lastTime > DELTA_TIME) {
			if (ship->move) {
				if (ship->direction == W) {
					ship->rectDst.x -= STEP;
					if (screen_limit(ship->rectDst)) ship->rectDst.x +=STEP;
				}
				if (ship->direction == E) {
					ship->rectDst.x += STEP;
					if (screen_limit(ship->rectDst)) ship->rectDst.x -=STEP;
				}
			}
		
		for (int i=0;i<NB_SPRITES_MISSILES;i++)
		{	
			ship->surfaceMissile[i]->clip_rect.y -= STEP;
		}
			
			if (ship->rectSrc.x == 0) ship->rectSrc.x = SHAPE_SIZE; // change sprite
				else ship->rectSrc.x = 0;
				
			update_scene(renderer, ship);
			lastTime = currentTime;
		}//endif DELTA_TIME
	}//end while
	
	free_ship(ship);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
