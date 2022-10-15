typedef struct {
	unsigned char** cube;			// cube dynamic array of arrays (Ex: cube[500][500])
	unsigned char** next;	// A copy of cube for update_one_time() blocks
	int W, H;						// With, Height
	int delay;						// SDL Delay
	int sp;							// Block spawn probability (x / 100) 
	int point_size;					// [NOT IMPLEMENTED] In cube
	int times;      // update times
} screen_t;