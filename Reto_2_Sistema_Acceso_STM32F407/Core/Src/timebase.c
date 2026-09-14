#include "timebase.h"

volatile uint32_t ms_ticks = 0;

// Configurar SysTick para 1ms (Asumiendo reloj del sistema a 16 MHz)
void SysTick_Init(void) {
    // 16 MHz / 1000 = 16000 ticks por milisegundo
    SysTick->LOAD = 16000 - 1;

    // Limpiar el valor actual
    SysTick->VAL = 0;

    // Habilitar SysTick, su interrupcion, y usar el reloj del procesador
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

// Rutina de Servicio de Interrupcion (ISR) del SysTick
void SysTick_Handler(void) {
    ms_ticks++; // Se incrementa cada 1 milisegundo
}
