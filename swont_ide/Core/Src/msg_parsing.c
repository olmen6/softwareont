/*
 * msg_parsing.c
 *
 *  Created on: 27 nov 2025
 *      Author: Jelle J
 */

#include "msg_parser.h"

/**
 * @brief functie om inkomende berichten te parsen voor de individuele commando's. run "argList args;" voor het aanmaken van de struct.
 * @param msg, de array om door te parsen
 * @param args, &adres van de struct waar de argumenten in moeten komen
 */
uint8_t parse(char *msg, argList *args)
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



