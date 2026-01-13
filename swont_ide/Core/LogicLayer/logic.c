/*
 * Melle Poeckling
 *
 * @file logic.c
 *
 * @brief Logic Layer tussen Front Layer en API Layer
 *
 * Dit .c bestand vertaald de commando's vanuit de Front Layer
 * naar de aanroepen in de API Layer.
 * De parameters worden hier gevalideerd en eventueel geconverteerd
 *
 *  */
#include "logic.h"
#include "api_draw_functies.h"
#include "../Inc/error.h"
#include <string.h>
#include <stdlib.h>

/**
 * @brief Zet kleurstring om naar VGA kleurcode
 *
 * @param color pointer naar kleurstring
 *
 * @return VGA kleurcode
 *
 * @note Onbekende kleurstring resulteren in default VGA_COL_YELLOW
 *
 */
static uint8_t color_from_string(const char *color)
{
	while (*color == ' ')
	    	color++;
    // Kleuren naam mapping
	if (strcmp(color, "wit") == 0)     return VGA_COL_WHITE;
    if (strcmp(color, "zwart") == 0)   return VGA_COL_BLACK;
    if (strcmp(color, "rood") == 0)    return VGA_COL_RED;
    if (strcmp(color, "groen") == 0)   return VGA_COL_GREEN;
    if (strcmp(color, "blauw") == 0)   return VGA_COL_BLUE;
    if (strcmp(color, "geel") == 0)    return VGA_COL_YELLOW;
    if (strcmp(color, "cyaan") == 0)   return VGA_COL_CYAN;
    if (strcmp(color, "magenta") == 0) return VGA_COL_MAGENTA;
    // fallback: gebruik geel

    return VGA_COL_YELLOW;
}

/**
 * @brief Vult het scherm met een solide kleur
 *
 * Valideert parameters uit de Front Layer en roept de onderliggende
 * API draw fucntie aan.
 *
 * @param color pointer naar kleurstring
 *
 * @return 0 bij fout, 1 bij succes
 *
 */
uint8_t logicAPICallfill(const char *color)
{
    if (color == NULL || strlen(color) == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Fill", .msg = "fill: kleur  NULL/leeg/ruimte" };
        Error_Report(&err);
        return 0;
    }
    uint8_t c = color_from_string(color);
    int res = API_draw_fill((int)c);
    return (res != 0) ? 1 : 0;
}

/**
 * @brief Tekent een lijn op het scherm
 *
 * @param x1p X-coördinaat startpunt
 * @param y1p Y-coördinaat startpunt
 * @param x2p X-coördinaat eindpunt
 * @param Y2p Y-coördinaat eindpunt
 * @param *color pointer naar kleurstring
 * @param siz lijndikte (>0)
 *
 * @return 0 bij fout en 1 bij succes
 *
 */
uint8_t logicAPICallLijn(uint16_t x1p, uint16_t y1p, uint16_t x2p, uint16_t y2p, const char *color, uint8_t siz)
{
    if (color == NULL || strlen(color) == 0 || siz == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Lijn", .msg = "lijn: color/size invalid" };
        Error_Report(&err);
        return 0;
    }

    if (x1p > MAX_X || y1p > MAX_Y || x2p > MAX_X || y2p > MAX_Y)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Lijn", .msg = "lijn: coordinates out of bounds" };
        Error_Report(&err);
        return 0;
    }
    int x1 = x1p; int y1 = y1p; int x2 = x2p; int y2 = y2p;
    uint8_t c = color_from_string(color);
    // Aanroepen van API functie
    // Returnt 1 bij succes
    int res = API_draw_line(x1, y1, x2, y2, (int)c, (int)siz, 0);
    return (res != 0) ? 1 : 0;
}

/**
 * @brief Tekent een rechthoek op het scherm
 *
 * @param x1p X-coördinaat startpunt
 * @param y1p Y-coördinaat startpunt
 * @param x2p X-coördinaat eindpunt
 * @param Y2p Y-coördinaat eindpunt
 * @param *color pointer naar kleurstring
 * @param filled 1 is gevuld, 0 is niet gevuld
 *
 * @return 0 bij fout en 1 bij succes
 *
 */
uint8_t logicAPICallRechthoek(uint16_t x1p, uint16_t y1p, uint16_t x2p, uint16_t y2p, const char *color, uint8_t filled)
{
    if (color == NULL || strlen(color) == 0 )
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Rechthoek", .msg = "rechthoek: kleur invalid" };
        Error_Report(&err);
        return 0;
    }
    if (x1p > MAX_X || y1p > MAX_Y || x2p > MAX_X || y2p > MAX_Y|| x1p >= MAX_X || y1p >= MAX_Y)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Rechthoek", .msg = "rechthoek: coordinates buiten bereik" };
        Error_Report(&err);
        return 0;
    }
    if (filled != 0 && filled != 1)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Rechthoek", .msg = "rechthoek: filled moet 0 or 1 zijn" };
        Error_Report(&err);
        return 0;
    }
    int x1 = x1p, y1 = y1p, x2 = x2p, y2 = y2p;
    uint8_t c = color_from_string(color);
    // Aanroepen van API functie
    // Returnt 1 bij succes
    int res = API_draw_rectangle(x1, y1, x2, y2, (int)c, (int)filled, 0, 0);
    return (res != 0) ? 1 : 0;
}

/**
 * @brief Tekent een cirkel op het scherm
 *
 * @param xp X-coördinaat middelpunt cirkel
 * @param yp Y-coördinaat middelpunt cirkel
 * @param siz radius van de cirkel
 * @param *color pointer naar kleurstring
 *
 * @return 0 bij fout en 1 bij succes
 *
 */
uint8_t logicAPICallcirkel(uint16_t xp, uint16_t yp, uint16_t siz, const char *color)
{

    if (color == NULL || strlen(color) == 0 || siz == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Cirkel", .msg = "cirkel: color of size invalide" };
        Error_Report(&err);
        return 0;
    }
    if (xp > MAX_X || yp > MAX_Y)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Cirkel", .msg = "cirkel: middelpunt out of bounds" };
        Error_Report(&err);
        return 0;
    }
    //  check om te bepalen of de cirkel op het scherm past
    if (xp - siz < 0 || xp + siz > MAX_X || yp - siz < 0 || yp + siz > MAX_Y)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Cirkel", .msg = "cirkel: radius komt out of bounds" };
        Error_Report(&err);
        return 0;
    }
    uint8_t c = color_from_string(color);
    // Aanroepen van API functie
    // Returnt 1 bij succes
    int res = API_draw_circle((int)xp, (int)yp, (int)siz, (int)c, 0);
    return (res != 0) ? 1 : 0;
}

/**
 * @brief Tekent een bitmap op het scherm
 *
 * @param bmpnr bitmapnummer (1-7)
 * @param xp X-coördinaat linkerbovenhoek
 * @param yp Y-coördinaat linkerbovenhoek
 *
 * @return 0 bij fout en 1 bij succes
 *
 */
uint8_t logicAPICallbitmap(uint8_t bmpnr, uint16_t xp, uint16_t yp)
{
    if (bmpnr < 0 || bmpnr > 6)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Bitmap", .msg = "bitmap: bmpnr buiten range (1-7)" };
        Error_Report(&err);
        return 0;
    }
    if (xp > MAX_X || yp > MAX_Y)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Bitmap", .msg = "bitmap: coördinaten out of bounds" };
        Error_Report(&err);
        return 0;
    }
    // Aanroepen van API functie
    // Returnt 1 bij succes
    int res = API_draw_bitmap((int)xp, (int)yp, (int)bmpnr);
    return (res != 0) ? 1 : 0;
}

/**
 * @brief Tekent tekst op het scherm
 *
 * @param xp X-coördinaat start tekst
 * @param yp Y-coördinaat start tekst
 * @param *color pointer naar kleurstring
 * @param *txtPrint tekststring om te printen
 * @param *font font type
 * @param siz fontsize (>0)
 * @param *modif modificatie (cursief, vet, normaal)
 *
 * @return 0 bij fout en 1 bij succes
 *
 */
uint8_t logicAPICalltxt(uint16_t xp, uint16_t yp, const char *color, const char *txtPrint, const char *font, uint16_t siz, const char *modif)
{

    if (color == NULL || strlen(color) == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Tekst", .msg = "text: Kleur invalid" };
        Error_Report(&err);
        return 0;
    }
    if (txtPrint == NULL || strlen(txtPrint) == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Tekst", .msg = "text: geen text om te printen" };
        Error_Report(&err);
        return 0;
    }
    if (font == NULL || strlen(font) == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Tekst", .msg = "text: ongeldig font" };
        Error_Report(&err);
        return 0;
    }
    if (modif == NULL || strlen(modif) == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Tekst", .msg = "text: ongeldige modifier" };
        Error_Report(&err);
        return 0;
    }
    if (siz == 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Tekst", .msg = "text: size moet > 0" };
        Error_Report(&err);
        return 0;
    }
    if (xp > MAX_X || yp > MAX_Y)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Tekst", .msg = "text: coördinaten out of bounds" };
        Error_Report(&err);
        return 0;
    }
    uint8_t c = color_from_string(color);
    //Aanroepen API functie
    //Return 1 bij succes
    int res = API_draw_text((int)xp, (int)yp, (int)c, txtPrint, font, (int)siz, modif, 0);
    return (res != 0) ? 1 : 0;
}

