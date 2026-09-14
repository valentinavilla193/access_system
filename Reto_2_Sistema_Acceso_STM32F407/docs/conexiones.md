# Tabla de conexiones

## Teclado matricial 4×4

| Señal | Pin STM32 | Configuración |
|---|---|---|
| F0 | PA0 | Output |
| F1 | PA1 | Output |
| F2 | PA2 | Output |
| F3 | PA3 | Output |
| C0 | PA4 | Input + Pull-Up |
| C1 | PA5 | Input + Pull-Up |
| C2 | PA6 | Input + Pull-Up |
| C3 | PA7 | Input + Pull-Up |

Las filas se activan una a una y las columnas se leen como entradas. Al utilizar pull-up, una columna activa es detectada en nivel bajo.

## Matriz LED 8×8

| Señal | Pin STM32 |
|---|---|
| C0 | PE0 |
| C1 | PE1 |
| C2 | PE2 |
| C3 | PE3 |
| C4 | PE4 |
| C5 | PE5 |
| C6 | PE6 |
| C7 | PE7 |
| F0 | PE8 |
| F1 | PE9 |
| F2 | PE10 |
| F3 | PE11 |
| F4 | PE12 |
| F5 | PE13 |
| F6 | PE14 |
| F7 | PE15 |

En el código actual se utiliza la polaridad:

- Fila activa en **BAJO**.
- Columna activa en **ALTO**.

## Correspondencia utilizada con matriz 1088AS

| Señal lógica | Pin físico 1088AS |
|---|---:|
| F0 / R1 | 9 |
| F1 / R2 | 14 |
| F2 / R3 | 8 |
| F3 / R4 | 12 |
| F4 / R5 | 1 |
| F5 / R6 | 7 |
| F6 / R7 | 2 |
| F7 / R8 | 5 |
| C0 / C1 | 13 |
| C1 / C2 | 3 |
| C2 / C3 | 4 |
| C3 / C4 | 10 |
| C4 / C5 | 6 |
| C5 / C6 | 11 |
| C6 / C7 | 15 |
| C7 / C8 | 16 |

> Antes de energizar otra referencia de matriz 8×8, debe verificarse su pinout y polaridad.
