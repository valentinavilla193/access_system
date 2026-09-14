#ifndef SISTEMA_H
#define SISTEMA_H

typedef enum {
    ESPERANDO,
    INGRESANDO,
    VALIDANDO,
    ACCEDIO,
    DENEGADO,
    PASS_DINAMICA
} Estados_principales_t;

void Sistema(void);

#endif 

