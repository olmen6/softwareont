/* logic.h - Logic layer API wrappers */
#ifndef LOGIC_LAYER_H
#define LOGIC_LAYER_H

#include <stdint.h>

uint8_t logicAPICallLijn(uint8_t x1p, uint8_t y1p, uint8_t x2p, uint8_t y2p, const char *color, uint8_t siz);
uint8_t logicAPICallfill(const char *color);
uint8_t logicAPICallRechthoek(uint8_t x1p, uint8_t y1p, uint8_t x2p, uint8_t y2p, const char *color, uint8_t filled);
uint8_t logicAPICallcirkel(uint8_t xp, uint8_t yp, uint8_t siz, const char *color);
uint8_t logicAPICallbitmap(uint8_t bmpnr, uint8_t xp, uint8_t yp);
uint8_t logicAPICalltxt(uint8_t xp, uint8_t yp, const char *color, const char *txtPrint, const char *font, uint8_t siz, const char *modif);

#endif // LOGIC_LAYER_H
