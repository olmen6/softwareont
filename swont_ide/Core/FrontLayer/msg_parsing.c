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
	args->count = 0;
	printing_done_flag = FALSE;
	
	char *tok = strtok(msg, ",");
	
	if (tok == NULL){
		{ Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "geen tokens in bericht" }; Error_Report(&err); }
		return (0);
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
	    	{ Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "geen argumenten" }; Error_Report(&err); }
	    	return (0);
    }


    const char *cmd = args->tokens[0];

	int i;
	for (i = 0; commands[i].name != NULL; i++) {
		if (strcmp(cmd, commands[i].name) == 0) {
			return commands[i].fn((argList *)args);
		}
	}
	// Error feedback bij onbekend commando
	{
		Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "Onbekend commando" };
		Error_Report(&err);
	}
	return (0); //error code onbekend argument
}


								/*Calling functies*/
/**
 * @brief functie om de logic layer aan te sturen (tekst)
 */
uint8_t call_tekst(argList *args)
{
	if (args->count < 8) {
		{ Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "tekst: te weinig argumenten" }; Error_Report(&err); }
		return 0;
	}
	uint8_t xp = atoi(args->tokens[1]);
	uint8_t yp = atoi(args->tokens[2]);
	char color[10]; strncpy(color, args->tokens[3], sizeof(color)-1); color[sizeof(color)-1] = '\0';
	char txtPrint[80]; strncpy(txtPrint, args->tokens[4], sizeof(txtPrint)-1); txtPrint[sizeof(txtPrint)-1] = '\0';
	char font[15]; strncpy(font, args->tokens[5], sizeof(font)-1); font[sizeof(font)-1] = '\0';
	uint8_t siz = atoi(args->tokens[6]);
	char modif[15]; strncpy(modif, args->tokens[7], sizeof(modif)-1); modif[sizeof(modif)-1] = '\0';
	return logicAPICalltxt(xp, yp, color, txtPrint, font, siz, modif);
}

/**
 * @brief functie om de logic layer aan te sturen (clearscherm)
 */
uint8_t call_fill(argList *args)
{
	if (args->count < 2) {
		{ Error_t err = { .layer = LAYER_APP, .code = ERR_PARAM, .module = "MsgParser", .msg = "fill: te weinig argumenten" }; Error_Report(&err); }
		return 0;
	}
	char color[10]; strncpy(color, args->tokens[1], sizeof(color)-1); color[sizeof(color)-1] = '\0';
	return logicAPICallfill(color);
}

/**
 * @brief functie om de logic layer aan te sturen (lijn)
 */
uint8_t call_lijn(argList *args)
{
	if (args->count < 7) return 0;
	uint8_t x1p = atoi(args->tokens[1]);
	uint8_t y1p = atoi(args->tokens[2]);
	uint8_t x2p = atoi(args->tokens[3]);
	uint8_t y2p = atoi(args->tokens[4]);
	char color[10]; strncpy(color, args->tokens[5], sizeof(color)-1); color[sizeof(color)-1] = '\0';
	uint8_t siz = atoi(args->tokens[6]);
	return logicAPICallLijn(x1p, y1p, x2p, y2p, color, siz);
}

/**
 * @brief functie om de logic layer aan te sturen (rechthoek)
 */
uint8_t call_rechthoek(argList *args)
{
	if (args->count < 7) return 0;
	uint8_t x1p = atoi(args->tokens[1]);
	uint8_t y1p = atoi(args->tokens[2]);
	uint8_t x2p = atoi(args->tokens[3]);
	uint8_t y2p = atoi(args->tokens[4]);
	char color[10]; strncpy(color, args->tokens[5], sizeof(color)-1); color[sizeof(color)-1] = '\0';
	uint8_t filled = atoi(args->tokens[6]);
	return logicAPICallRechthoek(x1p, y1p, x2p, y2p, color, filled);
}

/**
 * @brief functie om de logic layer aan te sturen (bitmap)
 */
uint8_t call_bitmap(argList *args)
{
	if (args->count < 4) return 0;
	uint8_t bmpnr = atoi(args->tokens[1]);
	uint8_t xp = atoi(args->tokens[2]);
	uint8_t yp = atoi(args->tokens[3]);
	return logicAPICallbitmap(bmpnr, xp, yp);
}

uint8_t call_wacht(argList *args)
{
	{ Error_t err = { .layer = LAYER_APP, .code = ERR_STATE, .module = "MsgParser", .msg = "wacht niet ondersteund" }; Error_Report(&err); }
	return 0;
}

uint8_t call_herhaal(argList *args)
{
	{ Error_t err = { .layer = LAYER_APP, .code = ERR_STATE, .module = "MsgParser", .msg = "herhaal niet ondersteund" }; Error_Report(&err); }
	return 0;
}

uint8_t call_cirkel(argList *args)
{
	if (args->count < 5) return 0;
	uint8_t xp = atoi(args->tokens[1]);
	uint8_t yp = atoi(args->tokens[2]);
	uint8_t siz = atoi(args->tokens[3]);
	char color[10]; strncpy(color, args->tokens[4], sizeof(color)-1); color[sizeof(color)-1] = '\0';
	return logicAPICallcirkel(xp, yp, siz, color);
}

uint8_t call_figuur(argList *args)
{
	{ Error_t err = { .layer = LAYER_APP, .code = ERR_STATE, .module = "MsgParser", .msg = "figuur niet geïmplementeerd" }; Error_Report(&err); }
	return 0;
}


