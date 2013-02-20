#include <avr/pgmspace.h>

#define RED strip.Color(255, 0, 0)
#define ORANGE strip.Color(255, 127, 0)
#define YELLOW strip.Color(255, 255, 0)
#define GREEN strip.Color(0, 255, 0)
#define BLUE strip.Color(0, 0, 255)
#define VIOLET strip.Color(143, 0, 255)

#define SKY_BLUE strip.Color(0, 95, 127)
#define NO_COLOR strip.Color(0,0,0)

/*
Complete map of pixel index to pixel x,y coordinates
Note that you call as pixels[y][x] 
The array works better this way and looks like the way
the strips are oriented.
*/
PROGMEM prog_uchar pixels[22][6] = 
{{21,22,65,66,109,110},
{20,23,64,67,108,111},
{19,24,63,68,107,112},
{18,25,62,69,106,113},
{17,26,61,70,105,114},
{16,27,60,71,104,115},
{15,28,59,72,103,116},
{14,29,58,73,102,117},
{13,30,57,74,101,118},
{12,31,56,75,100,119},
{11,32,55,76,99,120},
{10,33,54,77,98,121},
{9,34,53,78,97,122},
{8,35,52,79,96,123},
{7,36,51,80,95,124},
{6,37,50,81,94,125},
{5,38,49,82,93,126},
{4,39,48,83,92,127},
{3,40,47,84,91,128},
{2,41,46,85,90,129},
{1,42,45,86,89,130},
{0,43,44,87,88,131}};

//helper to get pixel index from x,y
inline uint8_t pixel(uint8_t x, uint8_t y) {
  return pgm_read_byte(&pixels[y][x]);
}

/*
precalculated distance of each pixel from the center,
point 2.5, 10.5
calculated via:
dist = sqrt(pow(x - 2.5, 4) + pow(y - 10.5, 2))
x-coord uses 4th power to compensate for the skewed 
dimensions of the strips
*/
PROGMEM prog_uchar vectors[22][6] = 
{{24, 21, 21, 21, 21, 24},
{22, 19, 19, 19, 19, 22},
{21, 17, 17, 17, 17, 21},
{19, 15, 15, 15, 15, 19},
{18, 13, 13, 13, 13, 18},
{16, 11, 11, 11, 11, 16},
{15, 10, 9, 9, 10, 15},
{14, 8, 7, 7, 8, 14},
{13, 6, 5, 5, 6, 13},
{12, 5, 3, 3, 5, 12},
{12, 4, 1, 1, 4, 12},
{12, 4, 1, 1, 4, 12},
{12, 5, 3, 3, 5, 12},
{13, 6, 5, 5, 6, 13},
{14, 8, 7, 7, 8, 14},
{15, 10, 9, 9, 10, 15},
{16, 11, 11, 11, 11, 16},
{18, 13, 13, 13, 13, 18},
{19, 15, 15, 15, 15, 19},
{21, 17, 17, 17, 17, 21},
{22, 19, 19, 19, 19, 22},
{24, 21, 21, 21, 21, 24}};

//helper method to get vector from x,y pixel coordinate
inline uint8_t vector(uint8_t x, uint8_t y) {
  return pgm_read_byte(&vectors[y][x]);
}
