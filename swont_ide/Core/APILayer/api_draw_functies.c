//8-12-2023, Olmen Langelaan
//api layer 
#include "api_draw_functies.h"
#include "../Inc/error.h"
#include "bitmaps.h"

extern char printing_done_flag;

int API_draw_line (int x1, int y1, int x2, int y2, 
int color, int weight, int reserved) 
{
	//check of coordinaten op het scherm liggen
	if((x1 < VGA_DISPLAY_X) && 
	(y1 < VGA_DISPLAY_Y) && 
	(x2 < VGA_DISPLAY_X) &&
	(y2 < VGA_DISPLAY_Y))
	{	
		//Bresenhams lijnalgoritme:( https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
		int dx, sx, dy, sy;
		dx = abs(x2 - x1);
		if (x1< x2)
			sx=1;
		else
			sx=-1;
		dy = 0-abs(y1-y2);
		if(y1 < y2)
			sy=1;
		else
			sy=-1;
		int error = dx+dy;


		for(int tel = 0;tel<maxloop;tel++)
		{
			//schrijf naar scherm
			UB_VGA_SetPixel(x1,y1,color);
			//breedte van de lijn
			
			//if ((y1 < y2-weight)||(x1 < x2-weight))
			//{	
	            for (int w_tel =1; w_tel <weight;w_tel++) 
				{	
					if(((y1+w_tel<=y2)||(x1+w_tel<=x2)))
					{
						UB_VGA_SetPixel((x1 + w_tel), y1, color);
	               		UB_VGA_SetPixel((x1), y1 + w_tel, color);
	               	}
	            }
	        //}
        	//naar volgende ding
			int e2 = 2*error;
			if (e2 >= dy)
			{
				if(x1 == x2)
					break;
				error = error +dy;
				x1 =x1 + sx;
			}
			if (e2 <= dx)
			{
				if (y1 == y2) 
					break;
				error = error + dx;
				y1= y1+sy;
			}
		}
		printing_done_flag = TRUE;		//klaar voor het volgende commando
		return 1; 	//lijn succesvol
	}
	else
	{
		printing_done_flag = TRUE; 		//klaar voor het volgende commando
		{ Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "VGA", .msg = "coordinaten buiten scherm" }; Error_Report(&err); }
		return 0; //error, de waardes liggen buiten het scherm
	}
}

int API_draw_rectangle (int x, int y,int width,int height, int color,int filled, int reserved, int reserved2)
{
	if(filled)
	{ //maak een vierkant
		for(int telx=0;telx<width;telx++)
			for(int tely=0;tely<height;tely++)
				UB_VGA_SetPixel(telx+x,tely+y,color);
	}else
	{
		for(int telx=0;telx<width;telx++)
		{
			UB_VGA_SetPixel(telx+x,y,color);
			UB_VGA_SetPixel(telx+x,y+height,color);
		}
		for(int ytel=0;ytel<height;ytel++)
		{
			UB_VGA_SetPixel(x,y+ytel,color);
			UB_VGA_SetPixel(width+x,y+ytel,color);
		}
	}
	printing_done_flag = TRUE;
	return 1;
}


int API_draw_bitmap(int x_lup, int y_lup, int bm_nr)
{
    const uint16_t* bm_adres;

    switch (bm_nr)
    {
        case 1:
            bm_adres=bitmap1;
            break;
        case 2:
            bm_adres=bitmap2;
            break;
        case 4:
        	bm_adres=bitmap4;
        	break;
        case 5:
        	bm_adres=bitmap5;
        	break;
        case 6:
        	bm_adres=bitmap6;
        	break;
        default:
            bm_adres=bitmap3;
            break;
    }

    for (int y=0;y<bm_height;y++)
    {
        for (int x=0;x<bm_width;x++)
        {
            int i=y * bm_height+x;
            uint16_t kleur=bm_adres[i];
            UB_VGA_SetPixel(x_lup+x,y_lup+y,
                kleur);
        }
    }

	printing_done_flag = TRUE;
	return 1;
}


const unsigned char font8x8_basic[96][8] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 32 ' '
	{0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x00}, // 33 '!'
	{0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00}, // 34 '"'
	{0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00}, // 35 '#'
	{0x18,0x3E,0x58,0x3C,0x1A,0x7C,0x18,0x00}, // 36 '$'
	{0xFF,0x0F,0xFF,0xFF,0x00,0x00,0x00,0xFF}, // 37 '%'
	{0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00}, // 38 '&'
	{0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00}, // 39 '''
	[40 ... 64] = {0},
	{0x18,0x24,0x42,0x7E,0x42,0x42,0x42,0x00}, // 65 'A'
	{0x7C,0x42,0x42,0x7C,0x42,0x42,0x7C,0x00}, // 66 'B'
	{0x3C,0x42,0x40,0x40,0x40,0x42,0x3C,0x00}, // 67 'C'
	{0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x00}, // 68 'D'
	{0x7E,0x40,0x40,0x7C,0x40,0x40,0x7E,0x00}, // 69 'E'
	{0x7E,0x40,0x40,0x7C,0x40,0x40,0x40,0x00}, // 70 'F'
	{0x3C,0x42,0x40,0x4E,0x42,0x42,0x3C,0x00}, // 71 'G'
	{0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x00}, // 72 'H'
	{0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, // 73 'I'
	{0x1E,0x08,0x08,0x08,0x08,0x48,0x30,0x00}, // 74 'J'
	[75 ... 95] = {0},
};

#include "../Inc/error.h"
int draw_char(int x,int y,int colour,char c,int scale)
{
	if ((c<32)||(c>127)) {
		Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "draw_char", .msg = "Niet-ondersteund karakter" };
		Error_Report(&err);
		return 0;//invalid karakter
	}
	const unsigned char*karakter=font8x8_basic[c - 32];
	// Check of karakter leeg is (alle bytes 0)
	int leeg = 1;
	for(int rij=0;rij<8;rij++)
		if(karakter[rij]) leeg = 0;
	if(leeg) {
		Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "draw_char", .msg = "Leeg font patroon" };
		Error_Report(&err);
		return 0;
	}
	for(int rij=0;rij<8;rij++)
	{
		unsigned char datarij=karakter[rij];
		for(int kolom=0;kolom<8;kolom++)
			if (datarij & (1 << (7 - kolom)))
			{
				UB_VGA_SetPixel(kolom + x, rij + y, colour);
			}
	}
	return 1;
}

int API_draw_text(int x_lup,int y_lup,int color,char*text,char*fontname,int fontsize,int fontstyle,int reserved)
{
    int cursor_x = x_lup;
    int cursor_y = y_lup;
    while (*text)
    {
        int out=draw_char(cursor_x,cursor_y,color,*text,fontsize);
        cursor_x+=(8* fontsize); 
        text++;
    }
	printing_done_flag = TRUE;
	return 1;
}

int API_draw_fill(int color)
{
	for (int y = 0; y < VGA_DISPLAY_Y; y++)
	{
		for (int x = 0; x < VGA_DISPLAY_X; x++)
		{
			UB_VGA_SetPixel(x, y, color);
		}
	}
	printing_done_flag = TRUE;
	return 1;
}

int API_draw_circle(int x_center, int y_center, int radius, int color, int reserved)
{
	if (radius <= 0) return 0;
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y) {
		UB_VGA_SetPixel(x_center + x, y_center + y, color);
		UB_VGA_SetPixel(x_center + y, y_center + x, color);
		UB_VGA_SetPixel(x_center - y, y_center + x, color);
		UB_VGA_SetPixel(x_center - x, y_center + y, color);
		UB_VGA_SetPixel(x_center - x, y_center - y, color);
		UB_VGA_SetPixel(x_center - y, y_center - x, color);
		UB_VGA_SetPixel(x_center + y, y_center - x, color);
		UB_VGA_SetPixel(x_center + x, y_center - y, color);

		y += 1;
		if (err <= 0) {
			err += 2*y + 1;
		}
		if (err > 0) {
			x -= 1;
			err -= 2*x + 1;
		}
	}
	printing_done_flag = TRUE;
	return 1;
}
