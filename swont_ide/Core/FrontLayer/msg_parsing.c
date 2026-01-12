/*
 * msg_parsing.c
 *
 *  Created on: 27 nov 2025
 *      Author: Jelle J
 */


#include "msg_parsing.h"
#include "logic.h"
#include "../Inc/error.h"
extern char printing_done_flag;

//lookup table setup
cmd_entry commands[] = {
		{TEKST, call_tekst},
		{RECHTHOEK, call_rechthoek},
		{LIJN, call_lijn},
		{BMP, call_bitmap},
		{CLEARSCHERM, call_fill},
		{WACHT, call_wacht},
		{CIRKEL, call_cirkel},
		{HERHAAL, call_herhaal},
		{FIGUUR, call_figuur},
		{NULL, NULL}			//einde, kon ook weglaten en checken voor \0 maar dat ging onhandig
};

/**
 * @brief functie om inkomende berichten te parsen voor de individuele commando's. run "argList args;" voor het aanmaken van de struct.
 * @param msg, de array om door te parsen
 * @param args, &adres van de struct waar de argumenten in moeten komen
 */
uint8_t parse_msg(char *msg, argList *args)
{
	if (msg == NULL || args == NULL)
	{
		Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "parse_msg: NULL pointer" };
		Error_Report(&err);
		printing_done_flag = TRUE;  // Must reset flag on error
		return 0;
	}

	args->count = 0;
	// Don't touch printing_done_flag here! Only API layer manages it
	
	// Make a working copy since strtok modifies the original
	char msg_copy[1024];
	strncpy(msg_copy, msg, sizeof(msg_copy) - 1);
	msg_copy[sizeof(msg_copy) - 1] = '\0';
	
	char *tok = strtok(msg_copy, ",");
	
	if (tok == NULL){
		Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "geen tokens in bericht" };
		Error_Report(&err);
		printing_done_flag = TRUE;  // Must reset flag on error
		return (0);
	}

	while (tok && args->count < MAX_ARG_COUNT){
		// Copy token string into the struct (safe copy)
		strncpy(args->tokens[args->count], tok, MAX_TOKEN_LEN - 1);
		args->tokens[args->count][MAX_TOKEN_LEN - 1] = '\0';  // Ensure null termination
		args->count++;
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
    if (args == NULL || args->count == 0)
    {
	    Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "process_msg: geen argumenten" };
	    Error_Report(&err);
	    printing_done_flag = TRUE;
	    return (0);
    }


    const char *cmd = args->tokens[0];

    if (cmd == NULL)
    {
        Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "process_msg: commando NULL" };
        Error_Report(&err);
        printing_done_flag = TRUE;  // Ensure flag is reset on error
        return 0;
    }

	int i;
    for (i = 0; commands[i].name != NULL; i++) {
        if (strcmp(cmd, commands[i].name) == 0) {
            // Command found, execute it. API layer will handle printing_done_flag
            return commands[i].fn((argList *)args);
        }
    }

    // Unknown command
    Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "onbekend commando" };
    Error_Report(&err);
    printing_done_flag = TRUE;  // Reset flag on unrecognized command
    return (0);
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
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "tekst: te weinig argumenten" };
			Error_Report(&err);
			return(0);
		} else if(args->count > 8){
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "tekst: te veel argumenten" };
			Error_Report(&err);
			return(0);
		}

	//integer argumenten
	uint8_t xp = 		atoi(args->tokens[1]);
	uint8_t yp = 		atoi(args->tokens[2]);
	uint8_t siz = 		atoi(args->tokens[6]);
	//string argumenten
	char color[10];
	strncpy(color, 		args->tokens[3], sizeof(color) - 1);
	color[sizeof(color) - 1] = '\0';
	char txtPrint[80];
	strncpy(txtPrint, 	args->tokens[4], sizeof(txtPrint) - 1);
	txtPrint[sizeof(txtPrint) - 1] = '\0';
	char font[15];
	strncpy(font, 		args->tokens[5], sizeof(font) - 1);
	font[sizeof(font) - 1] = '\0';
	char modif[15];
	strncpy(modif, 		args->tokens[7], sizeof(modif) - 1);
	modif[sizeof(modif) - 1] = '\0';

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
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "fill: te weinig argumenten" };
			Error_Report(&err);
			return(0);
		} else if(args->count > 2){
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "fill: te veel argumenten" };
			Error_Report(&err);
			return(0);
		}
	//kopiëer de string
	char color[10];
	strncpy(color, args->tokens[1], sizeof(color) - 1);
	color[sizeof(color) - 1] = '\0';

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
	uint16_t x1p = 	atoi(args->tokens[1]);
	uint16_t y1p = 	atoi(args->tokens[2]);
	uint16_t x2p = 	atoi(args->tokens[3]);
	uint16_t y2p = 	atoi(args->tokens[4]);
	uint16_t siz = 	atoi(args->tokens[6]);
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
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "rechthoek: te weinig argumenten" };
			Error_Report(&err);
			return(0);
		} else if(args->count > 7){
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "rechthoek: te veel argumenten" };
			Error_Report(&err);
			return(0);
		}

	//integer argumenten
	uint8_t x1p = 		atoi(args->tokens[1]);
	uint8_t y1p = 		atoi(args->tokens[2]);
	uint8_t x2p = 		atoi(args->tokens[3]);
	uint8_t y2p = 		atoi(args->tokens[4]);
	uint8_t filled = 	atoi(args->tokens[6]);
	//string argumenten
	char color[10];
	strncpy(color, 		args->tokens[5], sizeof(color) - 1);
	color[sizeof(color) - 1] = '\0';

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
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "bitmap: te weinig argumenten" };
			Error_Report(&err);
			return(0);
		} else if(args->count > 4){
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "bitmap: te veel argumenten" };
			Error_Report(&err);
			return(0);
		}

	uint8_t bmpnr = atoi(args->tokens[1]);
	uint8_t xp = 	atoi(args->tokens[2]);
	uint8_t yp =	atoi(args->tokens[3]);

	return logicAPICallbitmap(bmpnr, xp, yp);
}

/**
 * @brief functie om de logic layer aan te sturen (wacht)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_wacht(argList *args)
{
	{ Error_t err = { .layer = LAYER_APP, .code = ERR_STATE, .module = "MsgParser", .msg = "wacht niet ondersteund" }; Error_Report(&err); }
	return(0); 		//error: deze doen we niet
}

/**
 * @brief functie om de logic layer aan te sturen (herhaal)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_herhaal(argList *args)
{
	{ Error_t err = { .layer = LAYER_APP, .code = ERR_STATE, .module = "MsgParser", .msg = "herhaal niet ondersteund" }; Error_Report(&err); }
	return(0); 		//error deze doen we niet
}

/**
 * @brief functie om de logic layer aan te sturen (cirkel)
 * @param struct met pointers naar de argumenten in het ontvangen bericht (ln. 55 van msg_parser.h)
 */
uint8_t call_cirkel(argList *args)
{
	//check voor argument aantal
	if (args->count < 5){
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "cirkel: te weinig argumenten" };
			Error_Report(&err);
			return(0);
		} else if(args->count > 5){
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "cirkel: te veel argumenten" };
			Error_Report(&err);
			return(0);
		}

	//integer argumenten
	uint8_t xp  = atoi(args->tokens[1]);
	uint8_t yp  = atoi(args->tokens[2]);
	uint8_t siz = atoi(args->tokens[3]);
	//strings
	char color[10];
	strncpy(color, args->tokens[4], sizeof(color) - 1);
	color[sizeof(color) - 1] = '\0';

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
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "figuur: te weinig argumenten" };
			Error_Report(&err);
			return(0);
		} else if(args->count > 12){
			Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "figuur: te veel argumenten" };
			Error_Report(&err);
			return(0);
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
	uint8_t yp5 = atoi(args->tokens[10]);
	//string argumenten
	char color[10];
	strncpy(color, args->tokens[11], sizeof(color) - 1);
	color[sizeof(color) - 1] = '\0';

	// TODO: Implement figuur drawing with 5 points
	(void)xp1; (void)yp1; (void)xp2; (void)yp2; (void)xp3; (void)yp3; (void)xp4; (void)yp4; (void)xp5; (void)yp5; (void)color;
	Error_t err = { .layer = LAYER_APP, .code = ERR_STATE, .module = "MsgParser", .msg = "figuur: nog niet volledig geïmplementeerd" };
	Error_Report(&err);
	return 0;
}

