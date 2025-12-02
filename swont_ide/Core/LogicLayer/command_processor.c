/**
 ******************************************************************************
 * @file    command_processor.c
 * @brief   Command parser en processor implementatie
 * @details Ondersteund commando's:
 * 			- clearscreen
 * 			- lijn
 * 			- rechthoek
 * 			- tekst
 * 			- bitmap
 * 			- cirkel
 * 			- figuur
 * 			- wacht
 * 			- herhaal
 ******************************************************************************
 */


//			DIT IS NOG NIET AF

/* We gebruiken api_uart_printf voor alle output
 * Alle output gaat namelijk via API layer
*/
#define printf api_uart_printf				//Deze later aanmaken


/* Declaratie van de command handlers */
static command_result_t cmd_help(int argc, char **argv);
static command_result_t cmd_clearscherm(int argc, char **argv);
static command_result_t cmd_lijn(int argc, char **argv);
static command_result_t cmd_rechthoek(int argc, char **argv);
static command_result_t cmd_tekst(int argc, char **argv);
static command_result_t cmd_bitmap(int argc, char **argv);
static command_result_t cmd_cirkel(int argc, char **argv);
static command_result_t cmd_figuur(int argc, char **argv);
static command_result_t cmd_wacht(int argc, char **argv);
static command_result_t cmd_herhaal(int argc, char **argv);
//static command_result_t cmd_demo(int argc, char **argv);		//Demo optioneel (screensaver achtig)


/**
 * @brief Commando layout arrays
 *
 * WELLICHT IN HEADER FILE ZETTEN
 */
typedef struct {
    const char *name;
    command_result_t (*handler)(int argc, char **argv);
    const char *help_text;
} command_desc_t;

static const command_desc_t commands[] = {
    {"HELP", cmd_help, "HELP - Show available commands"},
    {"CLEARSCHERM", cmd_clearscherm, "CLEARSCHERM <color> - Clear screen with color"},
    {"LIJN", cmd_lijn, "LIJN <x1> <y1> <x2> <y2> <color> <thickness> - Draw line"},
    {"RECHTHOEK", cmd_rechthoek, "RECHTHOEK <x> <y> <w> <h> <color> <thickness> - Draw rectangle"},
    {"TEKST", cmd_tekst, "TEKST <x> <y> <color> <text> <font> <scale> <style> - Draw text"},
    {"BITMAP", cmd_bitmap, "BITMAP <id> <x> <y> - Draw bitmap at position"},
    {"CIRKEL", cmd_cirkel, "CIRKEL <x> <y> <radius> <color> - Draw circle"},
    {"FIGUUR", cmd_figuur, "FIGUUR <x1> <y1> <x2> <y2> ... <color> - Draw polygon"},
    {"WACHT", cmd_wacht, "WACHT <milliseconds> - Wait for specified time"},
    {"HERHAAL", cmd_herhaal, "HERHAAL <count> <command_id> - Repeat command"},
    {"DEMO", cmd_demo, "DEMO - Voer een demo van rechthoeken en clearscherm uit"},
    {NULL, NULL, NULL}
};




/**
 * @brief Namen van kleuren koppeling aan HEX waardes
 *
 * WELLICHT IN HEADER FILE ZETTEN
 */
typedef struct {
    const char *kleur;
    uint8_t hex_waarde;
} kleur_map_t;

static const kleur_map_t kleur_map[] = {
    {"zwart", 0x00},           /* Black */
    {"wit", 0xFF},             /* White */
    {"rood", 0xE0},            /* Red */
    {"groen", 0x1C},           /* Green */
    {"blauw", 0x03},           /* Blue */
    {"geel", 0xFC},            /* Yellow */
    {"cyan", 0x1F},            /* Cyan */
    {"cyaan", 0x1F},           /* Cyan (Dutch variant) */
    {"magenta", 0xE3},         /* Magenta */
    {"lichtblauw", 0x1F},      /* Light Blue (Cyan) */
    {"lichtcyaan", 0x1F},      /* Light Cyan */
    {"bruin", 0xA0},           /* Brown */
    {"grijs", 0x92},           /* Gray */
    {NULL, 0}
};


/**
 * @brief Conversie van kleur naam naar corresponderende HEX waarde
 * @return HEX kleur waarrde, of 0x00 als kleur niet gevonden is
 */
static uint8_t kleur_naar_hex(const char *naam)
{
    if (!naam) return 0x00;

    char naam_klein[32];
    strncpy(naam_klein, naam, sizeof(naam_klein) - 1);
    naam_klein[sizeof(naam_klein) - 1] = '\0';

    /* Zet om in kleine letters */
    for (int i = 0; naam_klein[i]; i++) {
        if (naam_klein[i] >= 'A' && naam_klein[i] <= 'Z') {
        	naam_klein[i] += 0x20;
        }
    }

    /* Doorzoek kleur_map_t */
    for (int i = 0; kleur_map[i].naam != NULL; i++) {
        if (strcmp(naam_klein, kleur_map[i].naam) == 0) {
            return kleur_map[i].hex_waarde;
        }
    }

    /* Probeer te parsen als hex */
    if (naam[0] == '0' && (naam[1] == 'x' || naam[1] == 'X')) {
        return (uint8_t)strtol(naam, NULL, 16);
    }

        /* Probeer te parsen als hex zonder 0x prefix (bijv, "FF", "E0") */
        if (strlen(naam) <= 2 && naam[0] != '\0' && isxdigit((unsigned char)naam[0])) {
            return (uint8_t)strtol(naam, NULL, 16);
        }

        return 0x00;
}

