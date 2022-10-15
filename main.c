#define TITLE "GoL - Hang Sun"

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "screen.h"



void update_one_time(screen_t* screen)
{
	#pragma omp parallel for
	for (int x = 0; x < screen->W; x++) {
		for (int y = 0; y < screen->H; y++) {
			int n = 0;
			int change[8][2] = {
				{ x-1 , y+1 },
				{ x   , y+1 },
				{ x+1 , y+1 },
				{ x-1 , y   },
				{ x+1 , y   },
				{ x-1 , y-1 },
				{ x   , y-1 },
				{ x+1 , y-1 }
			};
			for (int i = 0; i < 8; i++) {
				if (change[i][0] >= 0 && change[i][0] < screen->W
					&& change[i][1] >= 0 && change[i][1] < screen->H
					&& screen->cube[change[i][0]][change[i][1]] == 1)
				{
					n++;
				}
			}
			if (screen->cube[x][y]) {
				screen->next[x][y] = (n == 2) || (n == 3);
			}
			else {
				screen->next[x][y] = (n == 3);
			}
		}
	}
	#pragma omp parallel for
	for (int x = 0; x < screen->W; x++)	{
		memcpy(screen->cube[x], screen->next[x], screen->H * sizeof(unsigned char));
	}
}

void draw(screen_t* screen, SDL_Renderer* renderer)
{
	for (int x = 0; x < screen->W; x++) {
		for (int y = 0; y < screen->H; y++) {
			if (screen->cube[x][y]) {
				SDL_Rect rect;
				rect.x = x * screen->point_size;
				rect.y = y * screen->point_size;
				rect.w = screen->point_size;
				rect.h = screen->point_size;
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}



int main(int argc, char** argv){		// Inizialize
	int playtimes = 0;
	SDL_Init(SDL_INIT_VIDEO);
	screen_t* screen;
	if (argc==2)
	{
		screen = init_game_txt(argc, argv);
	}else if (argc==3) {
		screen = init_game_txt2(argc, argv);
	}
	else{
		screen = init_game(argc, argv);
	}

	SDL_Window* win = SDL_CreateWindow(TITLE, (screen->info.w >> 1)-(screen->W >> 1), (screen->info.h >> 1)-(screen->H >> 1),
										screen->W * screen->point_size, screen->H * screen->point_size, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event ev;


	unsigned char quit = 0, pause = 1, mouseDownLeft = 0, mouseDownRight = 0;
	while (!quit) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					switch (ev.key.keysym.sym) {
						case SDLK_ESCAPE:
						case SDLK_q:
							quit = 1;
							break;
						case SDLK_SPACE:
							pause = !pause;
							break;
						case SDLK_DOWN:
							screen->delay <<= 1;
							if (screen->delay > 500) screen->delay = 500;
							break;
						case SDLK_UP:
							screen->delay >>= 1;
							if (screen->delay < 1) screen->delay = 1;
							break;
					}
					break;						
				case SDL_MOUSEBUTTONDOWN:
					if (ev.button.button == SDL_BUTTON_LEFT) mouseDownLeft = 1;
					if (ev.button.button == SDL_BUTTON_RIGHT) mouseDownRight = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					if (ev.button.button == SDL_BUTTON_LEFT) mouseDownLeft = 0;
					if (ev.button.button == SDL_BUTTON_RIGHT) mouseDownRight = 0;
					break;
				case SDL_MOUSEMOTION: {
					int x_ = ev.button.x / screen->point_size;
					int y_ = ev.button.y / screen->point_size;
					if (x_ < screen->W && y_ < screen->H) {
						if (mouseDownLeft) {
							screen->cube[x_][y_] = 1;
						}
						else if (mouseDownRight) {
							screen->cube[x_][y_] = 0;
						}
					}
					break;
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 250, 240, 1);		// white
		if (!pause){
			update_one_time(screen);
			playtimes++;
		}
		
		draw(screen, renderer);
		SDL_SetRenderDrawColor(renderer, 105, 105, 105, 1);			// gray
		SDL_RenderPresent(renderer);
		if (playtimes==screen->times)
		{
			pause=1;
			playtimes=0;
		}
		if (pause) {
			SDL_Delay(1);
		}
		else {
			SDL_Delay(screen->delay);
		}
	}
	FILE *f_g = NULL;
	f_g = fopen("out.txt", "w");
	if (!f_g)
    {
        printf("Data load failure!\n");
        return 0;
    }
	fprintf(f_g, "%d\n%d\n", screen->W, screen->H);
	int i =0;
	int j =0;
	for (int i = 0; i < screen->W; i++)
	{
		for (int j = 0; j < screen->H; j++)
		{
			if (screen->cube[i][j]==1)
			{
			int a = i+1;
			int b = j+1;
			fprintf(f_g, "%d\n%d\n", a, b);
			}
			
		}
		
	}
	fclose(f_g);
	

	end_game(screen);			// Free all
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

//references：A MIT work on github