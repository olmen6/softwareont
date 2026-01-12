/*
 *
 * logic.c
 *
 * Logic layer voor aanroepen van API funcies naar aanleiding van Front Layer
 * 
 * Nog toe te voegen: error handling, validatie van parameters
 *                    Duidelijk commentaar per functie
 *
 *  */
#include "logic.h"
#include "api_draw_functies.h"
#include "../Inc/error.h"
#include <string.h>
#include <stdlib.h>

static uint8_t color_from_string(const char *color)
{
    if (color == NULL) return VGA_COL_BLACK;
    // If numeric string, use atoi
    if (color[0] >= '0' && color[0] <= '9'){
        return (uint8_t)atoi(color);
    }
    // Kleuren naam mapping
	if (strcmp(color, "wit") == 0)     return VGA_COL_WHITE;
    if (strcmp(color, "zwart") == 0)   return VGA_COL_BLACK;
    if (strcmp(color, "rood") == 0)    return VGA_COL_RED;
    if (strcmp(color, "groen") == 0)   return VGA_COL_GREEN;
    if (strcmp(color, "blauw") == 0)   return VGA_COL_BLUE;
    if (strcmp(color, "geel") == 0)    return VGA_COL_YELLOW;
    if (strcmp(color, "cyaan") == 0)   return VGA_COL_CYAN;
    if (strcmp(color, "magenta") == 0) return VGA_COL_MAGENTA;
    // fallback: use white
    return VGA_COL_YELLOW;
}


uint8_t logicAPICallfill(const char *color)
{
    if (color == NULL)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Logic", .msg = "fill: color NULL" };
        Error_Report(&err);
        return 0;
    }
    uint8_t c = color_from_string(color);
    int res = API_draw_fill((int)c);
    return (res != 0) ? 1 : 0;
}


uint8_t logicAPICallLijn(uint16_t x1p, uint16_t y1p, uint16_t x2p, uint16_t y2p, const char *color, uint8_t siz)
{
    if (color == NULL || siz <= 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Logic", .msg = "lijn: invalide parameters" };
        Error_Report(&err);
        return 0;
    }
    int x1 = x1p; int y1 = y1p; int x2 = x2p; int y2 = y2p;
    uint8_t c = color_from_string(color);
    // Use API layer to draw the line so LogicLayer does not depend on low-level
    // drawing implementation. API_draw_line returns non-zero on success.
    int res = API_draw_line(x1, y1, x2, y2, (int)c, (int)siz, 0);
    return (res != 0) ? 1 : 0;
}


uint8_t logicAPICallRechthoek(uint8_t x1p, uint8_t y1p, uint8_t x2p, uint8_t y2p, const char *color, uint8_t filled)
{
    if (color == NULL)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Logic", .msg = "rechthoek: color NULL" };
        Error_Report(&err);
        return 0;
    }
    uint8_t c = color_from_string(color);
    int x1 = x1p, y1 = y1p, x2 = x2p, y2 = y2p;
    int res = API_draw_rectangle(x1, y1, x2, y2, (int)c, (int)filled, 0, 0);
    return (res != 0) ? 1 : 0;
}

uint8_t logicAPICallcirkel(uint8_t xp, uint8_t yp, uint8_t siz, const char *color)
{
    if (color == NULL || siz <= 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Logic", .msg = "cirkel: invalide parameters" };
        Error_Report(&err);
        return 0;
    }
    uint8_t c = color_from_string(color);
    int res = API_draw_circle((int)xp, (int)yp, (int)siz, (int)c, 0);
    return (res != 0) ? 1 : 0;
}

uint8_t logicAPICallbitmap(uint8_t bmpnr, uint8_t xp, uint8_t yp)
{
    if (bmpnr < 1 || bmpnr > 7)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Logic", .msg = "bitmap: invalide bitmap nummer" };
        Error_Report(&err);
        return 0;
    }
    int res = API_draw_bitmap((int)xp, (int)yp, (int)bmpnr);
    return (res != 0) ? 1 : 0;
}

uint8_t logicAPICalltxt(uint8_t xp, uint8_t yp, const char *color, const char *txtPrint, const char *font, uint8_t siz, const char *modif)
{
    if (color == NULL || txtPrint == NULL || font == NULL || modif == NULL || siz <= 0)
    {
        Error_t err = { .layer = LAYER_MIDDLE, .code = ERR_PARAM, .module = "Logic", .msg = "text: invalide parameters" };
        Error_Report(&err);
        return 0;
    }
    uint8_t c = color_from_string(color);
    int res = API_draw_text((int)xp, (int)yp, (int)c, txtPrint, font, (int)siz, modif, 0);
    return (res != 0) ? 1 : 0;
}

