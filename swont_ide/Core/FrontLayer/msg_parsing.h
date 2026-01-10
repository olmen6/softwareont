/*
 * msg_parser.h
 *
 *  Created on: 27 nov 2025
 *      Author: Jelle J
 */

#ifndef INC_MSG_PARSER_H_
#define INC_MSG_PARSER_H_



#include "main.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

//functie tokens
#define TEKST "tekst"
#define LIJN "lijn"
#define CLEARSCHERM "clearscherm"
#define RECHTHOEK "rechthoek"
#define BMP "bitmap"
#define FIGUUR "figuur"
#define WACHT "wacht"
#define HERHAAL "herhaal"
#define CIRKEL "cirkel"
#define HELP "help"
#define MAX_ARG_COUNT 12

//functie prototypes
/**************************************************************************************************************************************/

uint8_t parse_msg();
uint8_t process_msg();
uint8_t call_tekst();
uint8_t call_rechthoek();
uint8_t call_lijn();
uint8_t call_bitmap();
uint8_t call_fill();
uint8_t call_wacht();
uint8_t call_cirkel();
uint8_t call_herhaal();
uint8_t call_figuur();

/**************************************************************************************************************************************/


//typedefs 
/**************************************************************************************************************************************/

//opgedeelde input array, elke opvolgende positie van tokens is de volgende token
typedef struct {
    char *tokens[MAX_ARG_COUNT];
    uint8_t count;		
} argList;	

//functie pointer typedef
typedef uint8_t (*cmd_fn)(argList *args);	

//look up table met de function pointers en commando namen
typedef struct {
    const char *name;		//naam van het commando
    cmd_fn fn;				//functiepointer naar de bijbehorende call functie
} cmd_entry;

/**************************************************************************************************************************************/

#endif /* INC_MSG_PARSER_H_ */
