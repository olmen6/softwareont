//8-12-2023, Olmen Langelaan
//api layer 
#include "api_draw_functies.h"

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
			//breete van de lijn
			for (int wx = -weight/2; wx <= weight/2; wx++) 
			{
            	for (int wy = -weight/2; wy <= weight/2; wy++) 
				{
            	    if (!(wx == 0 && wy == 0)) 
					{
                 	   UB_VGA_SetPixel((x1 + wx), y1 + wy, color);
                	}
            	}
        	}
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
		return 1;	//lijn succesvol
	}
	else
	{
		printing_done_flag = TRUE;		//klaar voor het volgende commando
		return 0; //error, de waardes liggen buiten het scherm
	}
}
