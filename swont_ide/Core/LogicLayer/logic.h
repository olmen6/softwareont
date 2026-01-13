/* logic.h - Logic layer naar API doorsturen/wrappers */
#ifndef LOGIC_LAYER_H
#define LOGIC_LAYER_H

#include <stdint.h>

#define MAX_X 319
#define MAX_Y 239

uint8_t logicAPICallLijn(uint16_t x1p, uint16_t y1p, uint16_t x2p, uint16_t y2p, const char *color, uint8_t siz);
uint8_t logicAPICallfill(const char *color);
uint8_t logicAPICallRechthoek(uint16_t x1p, uint16_t y1p, uint16_t x2p, uint16_t y2p, const char *color, uint8_t filled);
uint8_t logicAPICallcirkel(uint16_t xp, uint16_t yp, uint16_t siz, const char *color);
uint8_t logicAPICallbitmap(uint8_t bmpnr, uint16_t xp, uint16_t yp);
uint8_t logicAPICalltxt(uint16_t xp, uint16_t yp, const char *color, const char *txtPrint, const char *font, uint16_t siz, const char *modif);

#endif // LOGIC_LAYER_H
