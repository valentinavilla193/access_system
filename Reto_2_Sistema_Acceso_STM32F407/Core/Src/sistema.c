#include <stdbool.h>
#include "sistema.h"
#include "keypad.h"
#include "led_matrix.h"
#include "timebase.h"

#define ESPERA_FIGURA_MS 3000

static Estados_principales_t sistema_Estado = ESPERANDO;

static char ultimas_4_teclas[4] = {0};
static char contrasenia_Real[4] = {'A','B','C','D'};
static const char cambio_De_Clave[4] = {'*','*','*','*'};

static bool Comprobar_Contrasenia(const char a[4], const char b[4]) {
    for (int i = 0; i < 4; i++) {
        if (a[i] != b[i]) {
            return false; // Contraseña incorrecta
        }
    }
    return true; // Contraseña correcta
}

void Sistema(void) {
    char tecla = Keypad_Read_Debounced();
    static uint8_t i = 0;                   // digitos ingresados en INGRESANDO
    static uint8_t k = 0;                   // digitos ingresados en CONTRASENIA_DINAMICA
    static uint32_t tiempo_De_Espera_Imagen = 0;

    switch (sistema_Estado) {
        case ESPERANDO:
            if (tecla != '\0') {
                i = 0;
                ultimas_4_teclas[i++] = tecla;
                Led_Handler(LED_TECLA);
                sistema_Estado = INGRESANDO;
            }
            break;

        case INGRESANDO:
            if (tecla != '\0' && i < 4) {
                ultimas_4_teclas[i++] = tecla;
                Led_Handler(LED_TECLA);
                if (i >= 4) {
                    sistema_Estado = VALIDANDO;
                }
            }
            break;

        case VALIDANDO:
            if (Comprobar_Contrasenia(ultimas_4_teclas, cambio_De_Clave)) {
                sistema_Estado = CONTRASENIA_DINAMICA;
                k = 0;
            } else if (Comprobar_Contrasenia(ultimas_4_teclas, contrasenia_Real)) {
                sistema_Estado = ACCEDIO;
                Led_Handler(LED_ACCESO);
            } else {
                sistema_Estado = DENEGADO;
                Led_Handler(LED_DENEGADO);
            }
            tiempo_De_Espera_Imagen = ms_ticks;
            break;

        case ACCEDIO:
        case DENEGADO:
            if ((ms_ticks - tiempo_De_Espera_Imagen) >= ESPERA_FIGURA_MS) {
                Led_Handler(LED_NADA);
                sistema_Estado = ESPERANDO;
            }
            break;

        case CONTRASENIA_DINAMICA:
            if (tecla != '\0' && k < 4) {
                contrasenia_Real[k++] = tecla;
                Led_Handler(LED_TECLA);
                if (k >= 4) {
                    Led_Handler(LED_NADA);
                    sistema_Estado = ESPERANDO;
                }
            }
            break;

        default:
            sistema_Estado = ESPERANDO;
            break;
    }
}
