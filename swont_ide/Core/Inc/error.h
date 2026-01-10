/* Centrale foutafhandelingsmodule - error.h */
#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

typedef enum {
    ERR_OK = 0,
    ERR_PARAM,
    ERR_TIMEOUT,
    ERR_HW,
    ERR_STATE,
    ERR_UNKNOWN
} ErrorCode_t;

typedef enum {
    LAYER_HW = 0,
    LAYER_MIDDLE,
    LAYER_APP
} ErrorLayer_t;

typedef struct {
    ErrorLayer_t layer;
    ErrorCode_t  code;
    const char  *module;
    const char  *msg;
} Error_t;

void Error_Report(const Error_t *err);

const char *layerToStr(ErrorLayer_t l);
const char *errorToStr(ErrorCode_t e);

/* Map HAL status codes (use int to avoid including HAL headers here) */
ErrorCode_t HAL_StatusToErrorCode(int status);

#endif // ERROR_H
