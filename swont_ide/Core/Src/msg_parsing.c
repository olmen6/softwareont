/*
 * msg_parsing.c
 *
 *  Created on: 27 nov 2025
 *      Author: Jelle J
 */

#include "msg_parser.h"
#include "logic.h"

/**
 * @brief functie om inkomende berichten te parsen voor de individuele commando's. run "argList args;" voor het aanmaken van de struct.
 * @param msg, de array om door te parsen
 * @param args, &adres van de struct waar de argumenten in moeten komen
 */
uint8_t parse_msg(char *msg, argList *args)
{
	args->count = 0;
	
	char *tok = strtok(msg, ",");
	
	if (tok == NULL){
		return (0);		//pas aan zodra onze error handling meer vast ligt
	}

	while (tok && args->count < MAX_ARG_COUNT){
		args->tokens[args->count++] = tok;		//zet elke token in opvolgende posities van args.tokens
		tok = strtok(NULL, ",");
	}
	return (1);
}

/*
 * @brief functie om het gesplitste bericht te analyseren voor welk commando is aangevraagd. en roept vervolgens de bijbehorende functie aan
 * @param
 */
uint8_t process_msg(const argList *args)
{
    if (args->count == 0)
    {
    	return (0);		//error code geen argument
    }


    const char *cmd = args->tokens[0];

	int i;
    for (i = 0; commands[i].name != NULL; i++) {
        if (strcmp(cmd, commands[i].name) == 0) {
            return commands[i].fn((argList *)args);
        }
    }

    return (0); 		//error code onbekend argument
}


								/*Calling functies*/


/**
 * @brief functie om de logic layer aan te sturen (tekst)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_tekst(argList *args)
{
	//check voor argument aantal
	if (args->count < 8){
			return(0); 	//error code te weinig argumenten  	//TODO
		} else if(args->count > 8){
			return(0); 	//error code te veel argumenten 	//TODO
		}

	//integer argumenten
	uint8_t xp = 		atoi(args->tokens[1]);
	uint8_t yp = 		atoi(args->tokens[2]);
	uint8_t siz = 		atoi(args->tokens[6]);
	//string argumenten
	char color[10];
	strcpy(color, 		args->tokens[3]);
	char txtPrint[80];
	strcpy(txtPrint, 	args->tokens[4]);
	char font[15];
	strcpy(font, 		args->tokens[5]);
	char modif[15];
	strcpy(modif, 		args->tokens[7]);

	//API calling functie
	return logicAPICalltxt(xp, yp, color, txtPrint, font, siz, modif);
}

/**
 * @brief functie om de logic layer aan te sturen (clearscherm)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_fill(argList *args)
{
	//check voor argument aantal
	if (args->count < 2){
			return(0); 	//error code te weinig argumenten  	//TODO
		} else if(args->count > 2){
			return(0); 	//error code te veel argumenten 	//TODO
		}
	//kopiëer de string
	char color[10];
	strcpy(color, args->tokens[1]);

	//logicAPICallfill(color);								//TODO
	return logicAPICallfill(color);
}

/**
 * @brief functie om de logic layer aan te sturen (lijn)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_lijn(argList *args)
{
	//check voor argument aantal
	if (args->count < 7){
			return(0); 	//error code te weinig argumenten  	//TODO
		} else if(args->count > 7){
			return(0); 	//error code te veel argumenten 	//TODO
		}

	//integer argumenten
	uint8_t x1p = 	atoi(args->tokens[1]);
	uint8_t y1p = 	atoi(args->tokens[2]);
	uint8_t x2p = 	atoi(args->tokens[3]);
	uint8_t y2p = 	atoi(args->tokens[4]);
	uint8_t siz = 	atoi(args->tokens[6]);
	//string argumenten
	char color[10];
	strcpy(color, 	args->tokens[5]);

	//logicAPICallLijn(x1p, y1p, x2p, y2p, color, siz);		//TODO
	return logicAPICallLijn(x1p, y1p, x2p, y2p, color, siz);
}

/**
 * @brief functie om de logic layer aan te sturen (rechthoek)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_rechthoek(argList *args)
{
	//check voor argument aantal
	if (args->count < 7){
			return(0); 	//error code te weinig argumenten  	//TODO
		} else if(args->count > 7){
			return(0); 	//error code te veel argumenten 	//TODO
		}

	//integer argumenten
	uint8_t x1p = 		atoi(args->tokens[1]);
	uint8_t y1p = 		atoi(args->tokens[2]);
	uint8_t x2p = 		atoi(args->tokens[3]);
	uint8_t y2p = 		atoi(args->tokens[4]);
	uint8_t filled = 	atoi(args->tokens[6]);
	//string argumenten
	char color[10];
	strcpy(color, 		args->tokens[5]);

	//logicAPICallRechthoek(x1p, y1p, x2p, y2p, color, filled);	//TODO
	return logicAPICallRechthoek(x1p, y1p, x2p, y2p, color, filled);
}

/**
 * @brief functie om de logic layer aan te sturen (bitmap)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_bitmap(argList *args)
{
	//check voor argument aantal
	if (args->count < 4){
			return(0); 	//error code te weinig argumenten  	//TODO
		} else if(args->count > 4){
			return(0); 	//error code te veel argumenten 	//TODO
		}

	uint8_t bmpnr = atoi(args->tokens[1]);
	uint8_t xp = 	atoi(args->tokens[2]);
	uint8_t yp =	atoi(args->tokens[3]);

	//logicAPICallbitmap(bmpnr, xp, yp);					//TODO
	return logicAPICallbitmap(bmpnr, xp, yp);
}

/**
 * @brief functie om de logic layer aan te sturen (wacht)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_wacht(argList *args)
{
	return(0);		//error: deze doen we niet
}

/**
 * @brief functie om de logic layer aan te sturen (herhaal)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_herhaal(argList *args)
{
	return(0);		//error deze doen we niet
}

/**
 * @brief functie om de logic layer aan te sturen (cirkel)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_cirkel(argList *args)
{
	//check voor argument aantal
	if (args->count < 5){
			return(0); 	//error code te weinig argumenten  	//TODO
		} else if(args->count > 5){
			return(0); 	//error code te veel argumenten 	//TODO
		}

	//integer argumenten
	uint8_t xp  = atoi(args->tokens[1]);
	uint8_t yp  = atoi(args->tokens[2]);
	uint8_t siz = atoi(args->tokens[3]);
	//strings
	char color[10];
	strcpy(color, args->tokens[4]);

	//logicAPICallcirkel(xp, yp, siz, color);				//TODO
	return logicAPICallcirkel(xp, yp, siz, color);
}

/**
 * @brief functie om de logic layer aan te sturen (figuur)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_figuur(argList *args)
{
	//check voor argument aantal
	if (args->count < 12){
			return(0); 	//error code te weinig argumenten  	//TODO
		} else if(args->count > 12){
			return(0); 	//error code te veel argumenten 	//TODO
		}

	//integer argumenten
	uint8_t xp1 = atoi(args->tokens[1]);
	uint8_t yp1 = atoi(args->tokens[2]);
	uint8_t xp2 = atoi(args->tokens[3]);
	uint8_t yp2 = atoi(args->tokens[4]);
	uint8_t xp3 = atoi(args->tokens[5]);
	uint8_t yp3 = atoi(args->tokens[6]);
	uint8_t xp4 = atoi(args->tokens[7]);
	uint8_t yp4 = atoi(args->tokens[8]);
	uint8_t xp5 = atoi(args->tokens[9]);
	uint8_t yp5 = atoi(args->tokens[10]);	//dit kan nog wel wat schoner met een for loop
	//string argumenten
	char color[10];
	strcpy(color, args->tokens[11]);

	//logicAPICallFunc(xp1,yp1,xp2,yp2,xp3,yp3,xp4,yp4,xp5,yp5,color);	//TODO
	(void)xp1; (void)yp1; (void)xp2; (void)yp2; (void)xp3; (void)yp3; (void)xp4; (void)yp4; (void)xp5; (void)yp5; (void)color;
	return 0;
}

