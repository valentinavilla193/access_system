#ifndef TIMEBASE_H
#define TIMEBASE_H

#include "stm32f4xx.h"

/* Codigo BASE del curso (sin modificar): base de tiempo no bloqueante
 * por SysTick a 1 ms (asume reloj del sistema a 16 MHz). */

extern volatile uint32_t ms_ticks;

void SysTick_Init(void);

#endif /* TIMEBASE_H */
