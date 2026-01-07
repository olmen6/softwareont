/* Centrale foutafhandelingsmodule - error.c */
#include "../Inc/error.h"
#include "usart.h"
#include "string.h"
#include "stm32f4xx_hal.h"

static const char *layer_names[] = { "HW", "MIDDLE", "APP" };
static const char *error_names[] = { "ERR_OK", "ERR_PARAM", "ERR_TIMEOUT", "ERR_HW", "ERR_STATE", "ERR_UNKNOWN" };

const char *layerToStr(ErrorLayer_t l)
{
    if ((int)l < 0 || (int)l > 2) return "?";
    return layer_names[l];
}

const char *errorToStr(ErrorCode_t e)
{
    if ((int)e < 0 || (int)e > 5) return "?";
    return error_names[e];
}

static void UART_Print(const char *s)
{
    if (s == NULL) return;
    size_t len = strlen(s);
    if (len == 0) return;
    HAL_UART_Transmit(&huart2, (uint8_t *)s, (uint16_t)len, 1000);
}

void Error_Report(const Error_t *err)
{
    if (err == NULL) return;
    UART_Print("[");
    UART_Print(layerToStr(err->layer));
    UART_Print("][");
    UART_Print(errorToStr(err->code));
    UART_Print("][");
    UART_Print(err->module ? err->module : "?");
    UART_Print("] ");
    UART_Print(err->msg ? err->msg : "(geen bericht)");
    UART_Print("\r\n");
}

ErrorCode_t HAL_StatusToErrorCode(int status)
{
    switch (status) {
        case HAL_OK: return ERR_OK;
        case HAL_TIMEOUT: return ERR_TIMEOUT;
        case HAL_ERROR: return ERR_HW;
        default: return ERR_UNKNOWN;
    }
}
