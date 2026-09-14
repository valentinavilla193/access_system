#include "keypad.h"
#include "timebase.h" // para ms_ticks

#define DEBOUNCE_DELAY_MS 20

static KeypadState_t current_state = STATE_IDLE;
static uint32_t debounce_timer = 0;
static char last_valid_key = '\0';

/* --- Pines (confirmados por el estudiante) ----------------------------
 * Filas    F0-F3 -> PA0-PA3 (OUTPUT)
 * Columnas C0-C3 -> PA4-PA7 (INPUT con pull-up)
 * ----------------------------------------------------------------------*/

void Keypad_Init(void) {
    // Habilitar reloj para GPIOA (Bit 0)
    RCC->AHB1ENR |= (1 << 0);

    // Configurar PA0 a PA3 como salidas (Filas)
    GPIOA->MODER &= ~(0x000000FF); // Limpiar bits 0-7
    GPIOA->MODER |= 0x00000055;    // Establecer como 01 (General purpose output)

    // Configurar PA4 a PA7 como entradas (Columnas)
    GPIOA->MODER &= ~(0x0000FF00); // Limpiar bits 8-15 (00 = Input)

    // Activar resistencias Pull-Up internas para PA4-PA7
    GPIOA->PUPDR &= ~(0x0000FF00); // Limpiar registros PUPDR
    GPIOA->PUPDR |= 0x00005500;    // Establecer como 01 (Pull-up)
}

char Raw_Keypad_Scan(void) {
    const char keys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int row = 0; row < 4; row++) {
        GPIOA->BSRR = 0x000F;
        GPIOA->BSRR = (1 << (row + 16));

        for (volatile int i = 0; i < 50; i++);

        uint16_t cols = (GPIOA->IDR >> 4) & 0x0F;

        if (!(cols & 0x01)) return keys[row][0];
        if (!(cols & 0x02)) return keys[row][1];
        if (!(cols & 0x04)) return keys[row][2];
        if (!(cols & 0x08)) return keys[row][3];
    }

    return '\0';
}

char Keypad_Read_Debounced(void) {
    char raw_key = Raw_Keypad_Scan();
    char validated_key = '\0';

    switch (current_state) {
        case STATE_IDLE:
            if (raw_key != '\0') {
                last_valid_key = raw_key;
                debounce_timer = ms_ticks;
                current_state = STATE_DEBOUNCE_PRESS;
            }
            break;

        case STATE_DEBOUNCE_PRESS:
            if ((ms_ticks - debounce_timer) >= DEBOUNCE_DELAY_MS) {
                if (raw_key == last_valid_key) {
                    validated_key = last_valid_key;
                    current_state = STATE_PRESSED;
                } else {
                    current_state = STATE_IDLE;
                }
            }
            break;

        case STATE_PRESSED:
            if (raw_key == '\0') {
                debounce_timer = ms_ticks;
                current_state = STATE_DEBOUNCE_RELEASE;
            }
            break;

        case STATE_DEBOUNCE_RELEASE:
            if ((ms_ticks - debounce_timer) >= DEBOUNCE_DELAY_MS) {
                if (raw_key == '\0') {
                    current_state = STATE_IDLE;
                } else {
                    current_state = STATE_PRESSED;
                }
            }
            break;
    }

    return validated_key;
}

