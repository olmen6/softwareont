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
#define MAX_TOKEN_LEN 256  // Max length per token

#define TOKEN ","
#define TXTARGNUM 8
#define FILLARGNUM 2
#define LNARGNUM 7
#define RECARGNUM 7
#define BMPARGNUM 4
#define CIRKARGNUM 5
#define FIGARGNUM 12

//typedefs (moet voor prototypes!)
/**************************************************************************************************************************************/

//opgedeelde input array, elke opvolgende positie van tokens is de volgende token
typedef struct {
    char tokens[MAX_ARG_COUNT][MAX_TOKEN_LEN];  // Changed: store actual strings, not pointers
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

//functie prototypes
/**************************************************************************************************************************************/

uint8_t parse_msg(char *msg, argList *args);
uint8_t process_msg(const argList *args);
uint8_t call_tekst(argList *args);
uint8_t call_rechthoek(argList *args);
uint8_t call_lijn(argList *args);
uint8_t call_bitmap(argList *args);
uint8_t call_fill(argList *args);
uint8_t call_wacht(argList *args);
uint8_t call_cirkel(argList *args);
uint8_t call_herhaal(argList *args);
uint8_t call_figuur(argList *args);

/**************************************************************************************************************************************/

/**************************************************************************************************************************************/

#endif /* INC_MSG_PARSER_H_ */
