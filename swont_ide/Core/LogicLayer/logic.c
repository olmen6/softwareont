/*
 *
 *
 *
 *
 * logic.c
 *
 * Logic layer voor aanroepen van API funcies naar aanleiding van Front Layer
 *
 *
 *
 *  */
#include "logic.h"
#include "stm32_ub_vga_screen.h"
#include <string.h>
#include <stdlib.h>

static uint8_t color_from_string(const char *color)
{
    if (color == NULL) return VGA_COL_BLACK;
    // If numeric string, use atoi
    if (color[0] >= '0' && color[0] <= '9'){
        return (uint8_t)atoi(color);
    }
    // simple Dutch/English name mapping
    if (strcmp(color, "wit") == 0 || strcmp(color, "white") == 0) return VGA_COL_WHITE;
    if (strcmp(color, "zwart") == 0 || strcmp(color, "black") == 0) return VGA_COL_BLACK;
    if (strcmp(color, "rood") == 0 || strcmp(color, "red") == 0) return VGA_COL_RED;
    if (strcmp(color, "groen") == 0 || strcmp(color, "green") == 0) return VGA_COL_GREEN;
    if (strcmp(color, "blauw") == 0 || strcmp(color, "blue") == 0) return VGA_COL_BLUE;
    if (strcmp(color, "geel") == 0 || strcmp(color, "yellow") == 0) return VGA_COL_YELLOW;
    if (strcmp(color, "cyaan") == 0 || strcmp(color, "cyan") == 0) return VGA_COL_CYAN;
    if (strcmp(color, "magenta") == 0 || strcmp(color, "pink") == 0) return VGA_COL_MAGENTA;
    // fallback: use white
    return VGA_COL_WHITE;
}

uint8_t logicAPICallfill(const char *color)
{
    uint8_t c = color_from_string(color);
    UB_VGA_FillScreen(c);
    return 1;
}

uint8_t logicAPICallLijn(uint8_t x1p, uint8_t y1p, uint8_t x2p, uint8_t y2p, const char *color, uint8_t siz)
{
    int x1 = x1p; int y1 = y1p; int x2 = x2p; int y2 = y2p;
    uint8_t c = color_from_string(color);

    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        // draw pixel (size >1 -> draw small block)
        int xi, yi;
        int thickness = (siz > 1) ? siz : 1;
        for (yi = 0; yi < thickness; ++yi){
            for (xi = 0; xi < thickness; ++xi){
                int px = x1 + xi;
                int py = y1 + yi;
                if (px >= 0 && px < VGA_DISPLAY_X && py >= 0 && py < VGA_DISPLAY_Y)
                    UB_VGA_SetPixel(px, py, c);
            }
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
    return 1;
}

uint8_t logicAPICallRechthoek(uint8_t x1p, uint8_t y1p, uint8_t x2p, uint8_t y2p, const char *color, uint8_t filled)
{
    uint8_t c = color_from_string(color);
    int x1 = x1p, y1 = y1p, x2 = x2p, y2 = y2p;
    if (filled) {
        int x, y;
        for (y = y1; y <= y2; ++y){
            for (x = x1; x <= x2; ++x){
                UB_VGA_SetPixel(x, y, c);
            }
        }
        return 1;
    }
    // outline
    logicAPICallLijn(x1p, y1p, x2p, y1p, color, 1);
    logicAPICallLijn(x2p, y1p, x2p, y2p, color, 1);
    logicAPICallLijn(x2p, y2p, x1p, y2p, color, 1);
    logicAPICallLijn(x1p, y2p, x1p, y1p, color, 1);
    return 1;
}

uint8_t logicAPICallcirkel(uint8_t xp, uint8_t yp, uint8_t siz, const char *color)
{
    uint8_t c = color_from_string(color);
    int x0 = xp, y0 = yp, radius = siz;
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        UB_VGA_SetPixel(x0 + x, y0 + y, c);
        UB_VGA_SetPixel(x0 + y, y0 + x, c);
        UB_VGA_SetPixel(x0 - y, y0 + x, c);
        UB_VGA_SetPixel(x0 - x, y0 + y, c);
        UB_VGA_SetPixel(x0 - x, y0 - y, c);
        UB_VGA_SetPixel(x0 - y, y0 - x, c);
        UB_VGA_SetPixel(x0 + y, y0 - x, c);
        UB_VGA_SetPixel(x0 + x, y0 - y, c);

        y += 1;
        if (err <= 0) {
            err += 2*y + 1;
        } else {
            x -= 1;
            err -= 2*x + 1;
        }
    }
    return 1;
}

uint8_t logicAPICallbitmap(uint8_t bmpnr, uint8_t xp, uint8_t yp)
{
    // placeholder: no bitmap implementation available.
    (void)bmpnr; (void)xp; (void)yp;
    return 0;
}

uint8_t logicAPICalltxt(uint8_t xp, uint8_t yp, const char *color, const char *txtPrint, const char *font, uint8_t siz, const char *modif)
{
    // simple placeholder: draw a small rectangle to indicate text position
    uint8_t c = color_from_string(color);
    int w = 6 + (int)strlen(txtPrint);
    int h = 8;
    int x,y;
    for (y = 0; y < h; ++y){
        for (x = 0; x < w; ++x){
            int px = xp + x;
            int py = yp + y;
            if (px >=0 && px < VGA_DISPLAY_X && py >=0 && py < VGA_DISPLAY_Y)
                UB_VGA_SetPixel(px, py, c);
        }
    }
    (void)font; (void)siz; (void)modif;
    return 1;
}
