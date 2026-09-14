#include "stm32f4xx.h"
#include "timebase.h"
#include "keypad.h"
#include "led_matrix.h"
#include "sistema.h"

/* =======================================================================
 * Reto: Sistema de acceso con teclado matricial y matriz LED 8x8
 * -----------------------------------------------------
 * MAQUINA DE ESTADOS MAESTRA
 * -----------------------------------------------------
 * El propio while(1) actua como maquina maestra: en cada iteracion
 * ejecuta, en el mismo orden, a cada submaquina:
 *   1) Sistema()          -> ESPERANDO -> INGRESANDO -> VALIDANDO ->  ACCEDIO/DENEGADO.
 *   2) Matrix_ScanStep()  -> avanza un paso el multiplexado OUT-OUT de la matriz de LEDs.
 * Ambas se apoyan en 'ms_ticks' para evitar el bloqueo.
 * (base de tiempo de SysTick) para decidir cuando actuar.
 * ======================================================================= */
int main(void) {
    SysTick_Init();      // base de tiempo no bloqueante (SysTick a 1 ms)
    Keypad_Init();       // teclado (PA0-PA3 filas, PA4-PE7 columnas)
    Matrix_Init();       // matriz LED (PE0-PE15)
    Led_Handler(LED_NADA);// imagen inicial: sistema en espera

    while (1) {
        Sistema();

        Matrix_ScanStep();

    }
}
