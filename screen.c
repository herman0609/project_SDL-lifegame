#define DEF_POINT_SIZE 40
#define DEF_W 500;
#define DEF_H 500;
#define DEF_DELAY 100
#define DEF_SPAWN_PROBABILITY 0.00000
#define DEF_TIMES 0

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "screen.h"

unsigned int g_seed;

//to set the fast mode of the game
unsigned int fastrand() { 
	g_seed = 214013 * g_seed + 2531011; 
	return (g_seed >> 16) & 0x7FFF; 
} 
// the function of the fast mode and get a return value
unsigned int fast_mod(const unsigned int n, const unsigned int m)
{
	return (n < m ? n : n % m);
}

unsigned int rand_range(const unsigned int min, const unsigned int max)
{
    unsigned int r = fastrand();
    return fast_mod(r, max - min + 1) + min;
}

screen_t* init_game(int argc, char** argv)
{
	srand(time(NULL));
	g_seed = rand();

	screen_t* screen = (screen_t*) malloc(sizeof(screen_t));	// Allocate screen as pointer
	SDL_GetCurrentDisplayMode(0, &screen->info);				// Get screen info
	screen->point_size  = DEF_POINT_SIZE;
	screen->W			= DEF_W;
	screen->H			= DEF_H;
	screen->delay		= DEF_DELAY;
	screen->sp			= DEF_SPAWN_PROBABILITY;
	screen->times       = DEF_TIMES;

	if (argc >= 2 && argv[1][0] != '.' && argv[1][0] != 'IO') screen->W = atoi(argv[1]);
	if (argc >= 3 && argv[2][0] != '.') screen->H = atoi(argv[2]);
	if (argc >= 4 && argv[3][0] != '.') screen->delay = atoi(argv[3]);
	if (argc >= 5 && argv[4][0] != '.') screen->sp = atoi(argv[4]);
	if (argc >= 6 && argv[5][0] != '.') screen->point_size = 2*atoi(argv[5]);
	if (argc >= 7 && argv[6][0] != '.') screen->times = atoi(argv[6]);
	
	if (screen->point_size < 1) screen->point_size = 1;

	screen->W /= screen->point_size;
	screen->H /= screen->point_size;

	screen->cube      = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));
	screen->next = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));

	for (int x = 0; x < screen->W; x++) {
		screen->cube[x]	   = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		screen->next[x] = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		for (int y = 0; y < screen->H; y++) {
			screen->cube[x][y] = (rand_range(0, 100) <= screen->sp ? 1 : 0);
			screen->next[x][y] = screen->cube[x][y];
		}
	}

	return screen;
}

screen_t* init_game_txt(int argc, char** argv)
{
	srand(time(NULL));
	g_seed = rand();

	screen_t* screen = (screen_t*) malloc(sizeof(screen_t));	// Allocate screen as pointer
	SDL_GetCurrentDisplayMode(0, &screen->info);				// Get screen info
	screen->point_size  = DEF_POINT_SIZE;
	screen->delay		= DEF_DELAY;
	screen->sp			= DEF_SPAWN_PROBABILITY;

	

	if (argc >= 4 && argv[3][0] != '.') screen->delay = atoi(argv[3]);
	if (argc >= 5 && argv[4][0] != '.') screen->sp = atoi(argv[4]);
	if (argc >= 6 && argv[5][0] != '.') screen->point_size = 2*atoi(argv[5]);

	FILE *f_game = NULL;
	f_game = fopen("file.txt", "r");
	if (!f_game)
    {
        printf("Data load failure!\n");
        return 0;
    }
	int row;
	int column;
	if (fscanf(f_game, "%d", &row) != 1 || fscanf(f_game, "%d", &column) != 1)
    {
        printf("Wrong file!\n");
        return 0;
    }

	if (screen->point_size < 1) screen->point_size = 1;
	screen->W = row;
	screen->H = column;

	screen->cube      = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));
	screen->next = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));

	for (int x = 0; x < screen->W; x++) {
		screen->cube[x]	   = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		screen->next[x] = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		for (int y = 0; y < screen->H; y++) {
			screen->cube[x][y] = 0;
			screen->next[x][y] = screen->cube[x][y];
		}
	}

	int x=0;
	int y=0;
	while (fscanf(f_game, "%d", &x) == 1 && fscanf(f_game, "%d", &y) == 1)
	{
		for (int i = 0; i < screen->W; i++) {
			for (int j = 0; j < screen->H; j++) {
				if (x-1==i&&y-1==j)
				{
					screen->cube[x-1][y-1] = 1;
					screen->next[x-1][y-1] = screen->cube[x-1][y-1];
				}
			}
		}
	}
	

	return screen;

}

screen_t* init_game_txt2(int argc, char** argv)
{
	srand(time(NULL));
	g_seed = rand();
	
	screen_t* screen = (screen_t*) malloc(sizeof(screen_t));	// Allocate screen as pointer
	SDL_GetCurrentDisplayMode(0, &screen->info);				// Get screen info
	screen->point_size  = DEF_POINT_SIZE;
	screen->delay		= DEF_DELAY;
	screen->sp			= DEF_SPAWN_PROBABILITY;
	
	
	
	if (argc >= 4 && argv[3][0] != '.') screen->delay = atoi(argv[3]);
	if (argc >= 5 && argv[4][0] != '.') screen->sp = atoi(argv[4]);
	if (argc >= 6 && argv[5][0] != '.') screen->point_size = 2*atoi(argv[5]);
	
	FILE *f_game = NULL;
	f_game = fopen("out.txt", "r");
	if (!f_game)
		{
			printf("Data load failure!\n");
			return 0;
		}
	int row;
	int column;
	if (fscanf(f_game, "%d", &row) != 1 || fscanf(f_game, "%d", &column) != 1)
		{
			printf("Wrong file!\n");
			return 0;
		}
	
	if (screen->point_size < 1) screen->point_size = 1;
	screen->W = row;
	screen->H = column;
	
	screen->cube      = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));
	screen->next = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));
	
	for (int x = 0; x < screen->W; x++) {
		screen->cube[x]	   = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		screen->next[x] = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		for (int y = 0; y < screen->H; y++) {
			screen->cube[x][y] = 0;
			screen->next[x][y] = screen->cube[x][y];
		}
	}
	
	int x=0;
	int y=0;
	while (fscanf(f_game, "%d", &x) == 1 && fscanf(f_game, "%d", &y) == 1)
		{
			for (int i = 0; i < screen->W; i++) {
				for (int j = 0; j < screen->H; j++) {
					if (x-1==i&&y-1==j)
						{
							screen->cube[x-1][y-1] = 1;
							screen->next[x-1][y-1] = screen->cube[x-1][y-1];
						}
				}
			}
		}
	
	
	return screen;
	
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
