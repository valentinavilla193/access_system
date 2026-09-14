#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f4xx.h"

/* ---------------------------------------------------------------------
 * Codigo BASE del curso (sin modificar en su logica): driver del
 * teclado matricial 4x4 (multiplexacion IN-OUT) + maquina de estados
 * de antirrebote.
 * --------------------------------------------------------------------- */

typedef enum {
    STATE_IDLE,
    STATE_DEBOUNCE_PRESS,
    STATE_PRESSED,
    STATE_DEBOUNCE_RELEASE
} KeypadState_t;

void Keypad_Init(void);
char Raw_Keypad_Scan(void);
char Keypad_Read_Debounced(void);

#endif /* KEYPAD_H */
