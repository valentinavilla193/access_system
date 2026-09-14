#ifndef SISTEMA_H
#define SISTEMA_H

typedef enum {
    ESPERANDO,
    INGRESANDO,
    VALIDANDO,
    ACCEDIO,
    DENEGADO,
    CONTRASENIA_DINAMICA
} Estados_principales_t;

void Sistema(void);

#endif /* SISTEMA_H */
