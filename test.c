#define DEF_POINT_SIZE 40
#define DEF_W 500;
#define DEF_H 500;
#define DEF_DELAY 100
#define DEF_SPAWN_PROBABILITY 0.00000
#define DEF_TIMES 0

#include <stdio.h>
#include <string.h>
#include "Basic.h"
#include <SDL2/SDL.h>

typedef struct {
    unsigned char** cube;			// cube dynamic array of arrays (Ex: cube[500][500])
    unsigned char** next;	// A copy of cube for update_one_time() blocks
    int W, H;						// With, Height
    SDL_DisplayMode info;			// Screen info: Freq, width, height
    int delay;						// SDL Delay
    int sp;							// Block spawn probability (x / 100) 
    int point_size;					// [NOT IMPLEMENTED] In cube
    int times;
} screen_t;



unsigned int fast_mod(const unsigned int n, const unsigned int m)
{
    return (n < m ? n : n % m);
}

unsigned int g_seed;
unsigned int fastrand() { 
    g_seed = 214013 * g_seed + 2531011; 
    return (g_seed >> 16) & 0x7FFF; 
} 

unsigned int rand_range(const unsigned int min, const unsigned int max)
{
    unsigned int r = 101;
    return fast_mod(r, max - min + 1) + min;
}

int init_game()
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
    
    return screen->times;
}

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

static int suite_init(void)
{
    return 0;
}

static int suite_clean(void)
{
    return 0;
}

static void test_sum(void)
{
    CU_ASSERT_EQUAL(fast_mod(1, 2), 1);
    CU_ASSERT_EQUAL(fast_mod(5, 3), 2);
    CU_ASSERT_EQUAL(fast_mod(5, 8), 5);
    CU_ASSERT_EQUAL(fast_mod(5, 10), 5);
    CU_ASSERT_EQUAL(rand_range(4, 8), 5);
    CU_ASSERT_EQUAL(rand_range(3, 4), 4);
    CU_ASSERT_EQUAL(init_game(), 0);
    CU_ASSERT_EQUAL(suite_init(), 0);
    CU_ASSERT_EQUAL(suite_clean(), 0);
    CU_ASSERT_EQUAL(rand_range(4, 8), 5);
    CU_ASSERT_EQUAL(rand_range(3, 4), 4);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite("suite", suite_init, suite_clean);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "test", test_sum))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    /* Clean up registry and return */
    CU_cleanup_registry();
    return CU_get_error();
}
