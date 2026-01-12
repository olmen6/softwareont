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


int API_draw_text(int x_lup,int y_lup,int color,char*text,char*fontname,int fontsize,char *fontstyle,int reserved)
{
	while(*text)
	{
		
	    int index = *text - 32;
	    for (int row = 0; row < 8; row++)
	    {	
	    	unsigned char rowdata;
	    	if(strcmp(fontname, "arial") == 0)				//is de fontname arial?
	        	rowdata = arial[index][row];
	        else //if(strcmp(fontname, "consolas") == 0)   //is de fontname consolas
	        	rowdata=font2[index][row];	
	        //else return 0; //invalide fontnaam evt error code moet je wel if,else, return uncommenten____________________________
	        unsigned char b = rowdata;
	        for (int i = 7; i >= 0; i--)
	    	{
	        	//printf("%d ", (b >> i) & 1);
	        if((b>>i)&1)
	        {
	        	if(fontsize==1)//kleine text?
	        	{
	    			UB_VGA_SetPixel(7-i+x_lup,row+y_lup,color); 
	    			if(strcmp(fontstyle, "vet") == 0)
	    				UB_VGA_SetPixel(7-i+x_lup+1,row+y_lup,color); 
	    		}
	    		else //grote text?
	    			//if(fontsize==2) 
	    			{
	    				UB_VGA_SetPixel(15-i*fontsize+x_lup,(row*fontsize)+y_lup,color);//maak een dikke pixel
	    				UB_VGA_SetPixel(15-i*fontsize+x_lup+1,(row*fontsize)+y_lup,color);
	    				UB_VGA_SetPixel(15-i*fontsize+x_lup+1,(row*fontsize)+y_lup+1,color);
	    				UB_VGA_SetPixel(15-i*fontsize+x_lup,(row*fontsize)+y_lup+1,color);
	    				if(strcmp(fontstyle, "vet") == 0) //dikgedrukte text
	    				{
	    					UB_VGA_SetPixel(15-i*fontsize+x_lup+2,(row*fontsize)+y_lup+1,color);
	    					UB_VGA_SetPixel(15-i*fontsize+x_lup+2,(row*fontsize)+y_lup,color);
						}
	    			}
	    			//else 
	    			//return 0; //invalide fontsize   //evt error code dan moet je wel de if, else, return uncommenten __________________________
				}
			}
	    }
	    if((x_lup+= 8*fontsize)>(VGA_DISPLAY_X-8))	//gaat hij out of bounds?
	    	y_lup+=fontsize*8;						//volgende regel
	    x_lup= x_lup%(VGA_DISPLAY_X-8);				//zo ja? ga terug naar links
	    text++;
	}
	return 1; ///alles voltooid   XD
}
