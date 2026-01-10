//8-12-2023, Olmen Langelaan
//api layer 
#include "api_draw_functies.h"
#include "../Inc/error.h"

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

    return 1;
}


const unsigned char font8x8_basic[96][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 32 ' '
    {0x18,  /* 00011000 */
    0x18,  /* 00011000 */
    0x18,  /* 00011000 */
    0x18,  /* 00011000 */
    0x18,  /* 00011000 */
    0x00,  /* 00000000 */
    0x18,  /* 00011000 */
    0x00,  /* 00000000 */
}, // 33 '!'
    {0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00}, // 34 '"'
    {0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00}, // 35 '#'
    {0x18,0x3E,0x58,0x3C,0x1A,0x7C,0x18,0x00}, // 36 '$'
    {0xFF,0x0F,0xFF,0xFF,0x00,0x00,0x00,0xFF}, // 37 '%'
    {0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00}, // 38 '&'
    {0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00}, // 39 '''
    // ... vul aan als je meer tekens nodig hebt
};

int draw_char(int x,int y,int colour,char c,int scale)
{
    if ((c<32)||(c>127))
        return 0;//invalid karakter
    const unsigned char*karakter=font8x8_basic[c - 32];
	for(int rij=0;rij<8;rij++)
	{
		unsigned char datarij=karakter[rij];
		for(int kolom=0;kolom<8;kolom++)
			if(datarij&(1<<(7-rij)))
				UB_VGA_SetPixel(kolom+x,rij+y,colour);
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
    return 0;
}
