#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "screen.h"

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

void end_game(screen_t* screen)
{
	for (int i = 0; i < screen->W; i++) {
		free(screen->cube[i]);
		free(screen->next[i]);
	}
	free(screen->cube);
	free(screen->next);
	free(screen);
}