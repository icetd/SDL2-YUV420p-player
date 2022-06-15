#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define FRAME_WIDTH 	640
#define FRAME_HEIGHT	480

int main(int argc, char **argv)
{
	/*1 Initialization */
	SDL_Init(SDL_INIT_VIDEO);
	
	/*2 create window */
	SDL_Window *window = SDL_CreateWindow("SDL_mouse_move", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window) {
		perror("create window failed.");
		goto EXIT;
	}

	/*3 create readerer*/
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) {
		perror("create renderer failed.");
		goto EXIT;
	}
	
	/*4 create texture*/
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, 
			SDL_TEXTUREACCESS_TARGET, FRAME_WIDTH, FRAME_HEIGHT);
	if(!texture) {
		perror("create texture failed.");
	}

	SDL_Rect rect;
	rect.w = 40;
	rect.h = 40;	

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	int quit = 0;
	SDL_Event event;
	
	while(!quit)
	{
		SDL_WaitEvent(&event);

		switch(event.type) {
		case SDL_QUIT:
			quit = 1;
			break;

		case SDL_MOUSEMOTION:
			
			printf("mouse x:%d y:%d\n", 
					event.motion.x, event.motion.y);
			rect.x = event.motion.x;
			rect.y = event.motion.y;
			break;

		default:
			break;
		}

		/*5 Set a texture as the current rendering target*/
		SDL_SetRenderTarget(renderer, texture);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); //Set readerer color
		SDL_RenderClear(renderer); //Clear the current rendering target with the drawing color

		/*6 Draw a rectangle on the texture*/
		SDL_RenderDrawRect(renderer, &rect); 
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);//Fill a rectangle on the current rendering target with the drawing color.
		
		/*7 Set window as current rendering */
		SDL_SetRenderTarget(renderer, NULL);

		/*8 Cpoy the texture to the renderer*/
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		SDL_RenderPresent(renderer);
	}


EXIT:
	if(texture)
		SDL_DestroyTexture(texture);

	if(renderer)
		SDL_DestroyRenderer(renderer);

	if(window)
		SDL_DestroyWindow(window);
	
	SDL_Quit();

	return 0;
}
