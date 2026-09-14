#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "stm32f4xx.h"

typedef enum {
    LED_NADA,
    LED_TECLA,
    LED_ACCESO,
    LED_DENEGADO
} Estados_led;

void Matrix_Init(void);
void Matrix_ScanStep(void);
void Led_Handler(Estados_led estado);

#endif /* LED_MATRIX_H */
