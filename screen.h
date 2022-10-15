

#ifndef SCREEN_H
#define SCREEN_H


#include <SDL2/SDL.h>

typedef struct {
	unsigned char** cube;			// cube dynamic array of arrays (Ex: cube[500][500])
	unsigned char** next;	// A copy of cube for update_one_time() blocks
	int W, H;						// With, Height
	SDL_DisplayMode info;			// Screen info: Freq, width, height
	int delay;						// SDL Delay
	int sp;							// Block spawn probability (x / 100) 
	int point_size;					// [NOT IMPLEMENTED] In cube
	int times;      // update times
} screen_t;


unsigned int rand_range(const unsigned int min, const unsigned int max);
screen_t* init_game(int argc, char** argv);
//initialize the game when it is a no input version, the size is set as 12x12.
screen_t* init_game_txt(int argc, char** argv);
screen_t* init_game_txt2(int argc, char** argv);
//initialize the game with input file, the size is the first two integer form the file.
void end_game(screen_t* screen);
//To end the game when the game is ought to be end.

#endif