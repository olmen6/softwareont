#ifndef API_DRAW_FUNCTIES_H
#define API_DRAW_FUNCTIES_H

#include "stm32_ub_vga_screen.h"
#define maxloop 1000

int API_draw_line(int x1, int y1, int x2, int y2,
				  int color, int weight, int reserved);

int API_draw_rectangle(int x, int y, int width, int height,
					   int color, int filled, int reserved, int reserved2);

int API_draw_bitmap(int x_lup, int y_lup, int bm_nr);

int API_draw_text(int x_lup, int y_lup, int color, char *text, char *fontname,
				  int fontsize, int fontstyle, int reserved);

int draw_char(int x, int y, int colour, char c, int scale);

int API_draw_fill(int color);
int API_draw_circle(int x, int y, int radius, int color, int reserved);

#endif // API_DRAW_FUNCTIES_H
