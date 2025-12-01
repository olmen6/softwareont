/*
 * msg_parsing.c
 *
 *  Created on: 27 nov 2025
 *      Author: Jelle J
 */


/**
 * @brief functie om inkomende berichten te parsen voor de individuele commando's
 * @param msg, de array om door te parsen
 */

//#include "msg_parser.h"
#include <stdio.h>
#include <string.h>
#define TEKST "tekst"
#define MAX_ARG_COUNT 8

typedef struct {
    char *tokens[MAX_ARG_COUNT];
    int count;		//kan evt uint8_t aangezien we niet verder dan 8 hoeven tellen
} argList;

void parse(char *msg, argList *args)
{
	args->count = 0;
	
	char *tok = strtok(msg, ",");
	
	while (tok && args->count < MAX_ARG_COUNT){
		args->tokens[args->count++] = tok;		//zet elke token in opvolgende posities van args.tokens
		tok = strtok(NULL, ",");
	}
}

int main(void){
	char msg[] = "tekst,10,20, wit, the quick brown fox jumps over the lazy dog, consolas,1,normaal";
	
	argList args;
	parse(msg, &args);
	
	printf("Token count: %d\n", args.count);
	unsigned char i;
	for (i = 0; i < args.count; i++) {
        printf("Token %d: %s\n", i, args.tokens[i]);
    }
}


