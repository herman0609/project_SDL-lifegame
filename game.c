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
