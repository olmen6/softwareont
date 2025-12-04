/*
 * msg_parser.h
 *
 *  Created on: 27 nov 2025
 *      Author: Jelle J
 */

#ifndef INC_MSG_PARSER_H_
#define INC_MSG_PARSER_H_



#endif /* INC_MSG_PARSER_H_ */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

//functie tokens
#define TEXT "tekst"
#define LINE "lijn"
#define FILL "clearscherm"
#define RECT "rechthoek"
#define BMP "bitmap"
#define FIG "figuur"
#define WAIT "wacht"
#define REDO "herhaal"
#define MAX_ARG_COUNT 12

typedef struct {
    char *tokens[MAX_ARG_COUNT];
    int count;		//kan evt uint8_t aangezien we niet verder dan 8 hoeven tellen
} argList;

uint8_t parse();
