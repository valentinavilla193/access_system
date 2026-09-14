#include "led_matrix.h"

/* --- Pines: GPIOE completo (PE0-PE15) ---------------------------------
 * Puerto dedicado e independiente del teclado (que esta en GPIOA), asi
 * no hay riesgo de conflicto sin importar en que puerto quede el
 * teclado.
 *
 *   PE0-PE7  -> Columnas C0-C7 (byte bajo del ODR)
 *   PE8-PE15 -> Filas    F0-F7 (byte alto del ODR)
 *
 * Correspondencia con los pines fisicos del modulo 1088AS (numeracion
 * de 16 pines, pin 1 marcado con muesca/punto en la carcasa):
 *   F0=R1->pin9  F1=R2->pin14 F2=R3->pin8  F3=R4->pin12
 *   F4=R5->pin1  F5=R6->pin7  F6=R7->pin2  F7=R8->pin5
 *   C0=C1->pin13 C1=C2->pin3  C2=C3->pin4  C3=C4->pin10
 *   C4=C5->pin6  C5=C6->pin11 C6=C7->pin15 C7=C8->pin16
 * ----------------------------------------------------------------------*/

/* Polaridad de la matriz (confirmada por el estudiante con multimetro en
 * su modulo fisico): fila = catodo (se activa en BAJO), columna = anodo
 * (enciende en ALTO). Si en algun momento cambias de matriz y esto ya no
 * aplica, cambia este valor de 0 a 1 (fila=anodo/columna=catodo). */
#define FILA_ACTIVA_EN_ALTO 0

static uint8_t matrix_buffer[8];

/* Empaqueta 8 bytes (uno por fila) en un uint64_t, para poder escribir
 * los patrones en binario/hex legible sin calcular el numero combinado
 * a mano. */
#define LED_IMG(r0,r1,r2,r3,r4,r5,r6,r7) \
    ( ((uint64_t)(uint8_t)(r0))       | ((uint64_t)(uint8_t)(r1) << 8)  | \
      ((uint64_t)(uint8_t)(r2) << 16) | ((uint64_t)(uint8_t)(r3) << 24) | \
      ((uint64_t)(uint8_t)(r4) << 32) | ((uint64_t)(uint8_t)(r5) << 40) | \
      ((uint64_t)(uint8_t)(r6) << 48) | ((uint64_t)(uint8_t)(r7) << 56) )

/* Los indices designados ([LED_NADA] = ...) hacen que el orden no
 * dependa de como este declarado el enum Estados_led. */
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
