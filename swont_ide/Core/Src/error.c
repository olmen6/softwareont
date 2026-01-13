/*
 * Melle Poeckling
 *
 * @file error.c
 *
 * @brief Centrale error handling
 *
 * Implementeert gestructyureerde foutafhandeling over de hele 3 lagen applicatie.
 * Geeft de laag aan, de soort fout, en de module (functie in laag)
 *
 *  */



#include "../Inc/error.h"
#include "usart.h"
#include "string.h"
#include "stm32f4xx_hal.h"

static const char *layer_names[] = { "HW", "MIDDLE", "APP" };
static const char *error_names[] = { "ERR_OK", "ERR_PARAM", "ERR_TIMEOUT", "ERR_HW", "ERR_STATE", "ERR_UNKNOWN" };


/**
 * @brief Converteert ErrorLayer_t enum naar string representatie
 *
 * @param l ErrorLayer_t waarde
 *
 * @return pointer naar layer naam string, "?" bij ongeldig
 *
 */
const char *layerToStr(ErrorLayer_t l)
{
    if ((int)l < 0 || (int)l > 2)
    	return "?";
    return layer_names[l];
}

/**
 * @brief Converteert ErrorCode_t enum naar string representatie
 *
 * @param e ErrorCode_t waarde
 *
 * @return pointer naar error code naam string, "?" bij ongeldig
 *
 */
const char *errorToStr(ErrorCode_t e)
{
    if ((int)e < 0 || (int)e > 5)
    	return "?";
    return error_names[e];
}

/**
 * @brief Stuurt string via UART2
 *
 * @param s pointer naar string om te sturen
 *
 * @note Controleert op NULL pointer en lege strings
 *
 */
static void UART_Print(const char *s)
{
    if (s == NULL)
    	return;
    size_t len = strlen(s);
    if (len == 0)
    	return;
    HAL_UART_Transmit(&huart2, (uint8_t *)s, (uint16_t)len, 1000);
}

/**
 * @brief Rapporteert fout via UART in gestandaardiseerd formaat
 *
 * Format: [LAYER][CODE][MODULE] message\r\n
 * Voorbeeld: [MIDDLE][ERR_PARAM][Logic] lijn: kleur invalid\r\n
 *
 * @param err pointer naar Error_t struct met foutgegevens
 *
 * @note Controleert op NULL en stuurt altijd UART feedback
 *
 */
void Error_Report(const Error_t *err)
{
    if (err == NULL)
    	return;
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

/**
 * @brief Converteert HAL status codes naar ErrorCode_t
 *
 * @param status HAL status waarde (HAL_OK, HAL_TIMEOUT, HAL_ERROR, etc)
 *
 * @return overeenkomende ErrorCode_t waarde
 *
 */
ErrorCode_t HAL_StatusToErrorCode(int status)
{
    switch (status) {
        case HAL_OK:
        	return ERR_OK;
        case HAL_TIMEOUT:
        	return ERR_TIMEOUT;
        case HAL_ERROR:
        	return ERR_HW;
        default:
        	return ERR_UNKNOWN;
    }
}
