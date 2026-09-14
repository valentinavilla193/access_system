#include "stm32f4xx.h"
#include "timebase.h"
#include "keypad.h"
#include "led_matrix.h"
#include "sistema.h"

/* =======================================================================
 * Reto: Sistema de acceso con teclado matricial y matriz LED 8x8
 * STM32F407VET6 - Programacion Bare-Metal (sin HAL/LL)
 *
 * MAQUINA DE ESTADOS MAESTRA (seccion 14 del enunciado)
 * -----------------------------------------------------
 * El propio while(1) actua como maquina maestra: en cada iteracion
 * ejecuta, en el mismo orden, a cada submaquina:
 *   1) Sistema()          -> internamente llama a Keypad_Read_Debounced()
 *                             (teclado + antirrebote) y resuelve la logica
 *                             de ingreso/validacion de contraseña.
 *   2) Matrix_ScanStep()  -> avanza un paso el multiplexado OUT-OUT de
 *                             la matriz de LEDs.
 * Ninguna de las dos bloquea el programa: ambas se apoyan en 'ms_ticks'
 * (base de tiempo de SysTick) para decidir cuando actuar.
 * ======================================================================= */
int main(void) {
    SysTick_Init();      // base de tiempo no bloqueante (SysTick a 1 ms)
    Keypad_Init();       // driver de teclado (PA0-PA3 filas, PA4-PA7 columnas)
    Matrix_Init();       // driver de matriz LED (PE0-PE15)
    Led_Handler(LED_NADA); // imagen inicial: sistema en espera

    while (1) {
        Sistema();
        Matrix_ScanStep();
    }
}
