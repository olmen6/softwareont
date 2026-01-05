#include "stm32_ub_vga_screen.h"
#include "bitmaps.h"
#define maxloop 1000
int API_draw_line (int x1, int y1, int x2, int y2,
					int color, int weight, int reserved);
int API_draw_rectangle (int x, int y,int width,int height, int color,
						int filled, int reserved, int reserved2);
int API_draw_bitmap(int x_lup, int y_lup, int bm_nr);
