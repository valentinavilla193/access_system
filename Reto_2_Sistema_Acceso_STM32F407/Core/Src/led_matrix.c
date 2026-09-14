#include "led_matrix.h"

/* --- Pines: GPIOE (PE0-PE15) ---------------------------------
 *   PE0-PE7  -> Columnas C0-C7 (byte bajo del ODR)
 *   PE8-PE15 -> Filas    F0-F7 (byte alto del ODR)
 *
 * Correspondencia con los pines del modulo 1088AS (numeracion de 16 pines, pin 1 marcado con muesca):
 *   F0=R1->(9)  F1=R2->(14) F2=R3->(8)  F3=R4->(12) F4=R5->(1)  F5=R6->(7)  F6=R7->(2)  F7=R8->(5)
 *   C0=C1->(13) C1=C2->(3)  C2=C3->(4)  C3=C4->(10) C4=C5->(6)  C5=C6->(11) C6=C7->(15) C7=C8->(16)
 *
 * Polaridad confirmada con multimetro: fila = catodo (se activa en BAJO),
 * columna = anodo (enciende en ALTO).
 * ----------------------------------------------------------------------*/

static uint8_t matrix_buffer[8];

// Empaqueta 8 bytes (uno por fila) en un uint64_t, para calcular el valor
// empacado sin tener que sumar hexadecimales a mano.
#define LED_IMG(r0,r1,r2,r3,r4,r5,r6,r7) \
    ( ((uint64_t)(uint8_t)(r0))       | ((uint64_t)(uint8_t)(r1) << 8)  | \
      ((uint64_t)(uint8_t)(r2) << 16) | ((uint64_t)(uint8_t)(r3) << 24) | \
      ((uint64_t)(uint8_t)(r4) << 32) | ((uint64_t)(uint8_t)(r5) << 40) | \
      ((uint64_t)(uint8_t)(r6) << 48) | ((uint64_t)(uint8_t)(r7) << 56) )

#define LED_IMG(r0,r1,r2,r3,r4,r5,r6,r7) \
    ( ((uint64_t)(uint8_t)(r0))       | ((uint64_t)(uint8_t)(r1) << 8)  | \
      ((uint64_t)(uint8_t)(r2) << 16) | ((uint64_t)(uint8_t)(r3) << 24) | \
      ((uint64_t)(uint8_t)(r4) << 32) | ((uint64_t)(uint8_t)(r5) << 40) | \
      ((uint64_t)(uint8_t)(r6) << 48) | ((uint64_t)(uint8_t)(r7) << 56) )

static const uint64_t patrones_led_fijos[4] = {
    [LED_NADA]     = LED_IMG(0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C), /* "carita" de espera */
    [LED_TECLA]    = LED_IMG(0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00), /* punto central */
    [LED_ACCESO]   = LED_IMG(0x00,0x01,0x02,0x04,0x48,0x90,0x20,0x00), /* check aproximado */
    [LED_DENEGADO] = LED_IMG(0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81), /* X */
};

void Matrix_Init(void) {
    RCC->AHB1ENR |= (1 << 4);   // Habilita reloj de GPIOE (bit 4)
    GPIOE->MODER  = 0x55555555; // PE0-PE15 como salida (01 repetido 16 veces)
    GPIOE->OTYPER = 0x0000;     // push-pull
    GPIOE->ODR    = 0x0000;     // todo apagado al inicio
}

void Matrix_ScanStep(void) {
    static uint8_t fila = 0;
    uint8_t patron = matrix_buffer[fila]; // bit=1 significa "LED encendido"

#if FILA_ACTIVA_EN_ALTO
    uint16_t columnas = (uint16_t)(~patron) & 0xFF;              // columna en BAJO enciende (catodo)
    uint16_t filas    = (uint16_t)(1u << fila) << 8;             // fila activa en ALTO (anodo)
#else
    uint16_t columnas = (uint16_t)patron;                        // columna en ALTO enciende (anodo)
    uint16_t filas    = (uint16_t)(~(1u << fila) & 0xFF) << 8;   // fila activa en BAJO (catodo)
#endif

    GPIOE->ODR = columnas | filas; // Enciende la fila actual y apaga las demas
    fila = (fila + 1) % 8;
}

void Led_Handler(Estados_led estado) {
    uint64_t img = patrones_led_fijos[estado];
    for (int i = 0; i < 8; i++) {
        matrix_buffer[i] = (img >> (i * 8)) & 0xFF;
    }
}
