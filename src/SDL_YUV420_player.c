#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define TESTFILE		"../../test_video/640_360_test.yuv"

#define FRAME_WIDTH 	640
#define FRAME_HEIGHT	360
#define FRAMESIZE		((FRAME_HEIGHT) * (FRAME_WIDTH) * 3/2)

#define REFRESH_TIME_EVENT (SDL_USEREVENT + 1)

static int time_exit;

int refresh_timer(void *arg)
{
	time_exit = 0;

	while(!time_exit)
	{
		SDL_Event event;
		event.type = REFRESH_TIME_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(10);
	}

	time_exit = 1;

	return 0;
}

void yuvplayer(const char *fileName, int width, int height)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	SDL_Rect rect;
	
	SDL_Event event;
	SDL_Thread *time_thread;

	FILE *file = NULL;
	int quit = 0;
	int read_len;
	unsigned char *buf = NULL;


	buf = malloc(FRAMESIZE * sizeof(char));
	if(buf == NULL) {
		perror("malloc error.");
		goto EXIT;
	}
	SDL_Init(SDL_INIT_VIDEO);
	
	window = SDL_CreateWindow("SDL_YUV420_player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_OPENGL);
	if(!window) {
		perror("SDL_CreateWindow failed.");
		goto EXIT;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer) {
		perror("SDL_CreateRenderer failed.");
		goto EXIT;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, 
			SDL_TEXTUREACCESS_STREAMING, width, height);
	if(!texture) {
		perror("SDL_CreateTexture failed.");
		goto EXIT;
	}

	file = fopen(fileName, "r");
	if(file == NULL) {
		perror("open file failed.");
		goto EXIT;
	}

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	time_thread = SDL_CreateThread(refresh_timer, "timer", NULL);
	while(!quit)
	{
		SDL_WaitEvent(&event);

		switch(event.type) {
		
		case SDL_QUIT:
			quit = 1;
			time_exit = 1;
			break;

		case REFRESH_TIME_EVENT:
			read_len = fread(buf, 1, FRAMESIZE, file);
			if(read_len == 0) {
				perror("file end:");
				goto EXIT;
			}

			SDL_UpdateTexture(texture, NULL, buf, width);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
			break;

		default:
			break;
		}
	}

EXIT:
	time_exit = 1;

	if(file != NULL)
		fclose(file);

	if(buf)
		free(buf);

	if(texture)
		SDL_DestroyTexture(texture);

	if(renderer)
		SDL_DestroyRenderer(renderer);

	if(window)
		SDL_DestroyWindow(window);
	
	SDL_Quit();

	return ;
}


int main(int argc, char **argv)
{
	char *fileName = TESTFILE;
	int video_width = FRAME_WIDTH;
	int video_height = FRAME_HEIGHT;
	
	if((argc != 1) && (argc != 4)) {
		printf("Usage :%s <fileName> <video width> <video height>\n", argv[0]);
		printf("Or \nUsage:%s <fileName> \n", argv[0]);
		return -1;
	}
	
	if(argc == 4)
	{
		fileName = argv[1];
		video_height =atoi(argv[2]);
		video_width = atoi(argv[3]);
	}
	
	yuvplayer(fileName, video_width, video_height);
	return 0;
}
