# Diagramas de estados

## 1. Máquina de estados del sistema

```mermaid
stateDiagram-v2
    [*] --> ESPERANDO

    ESPERANDO --> INGRESANDO: tecla válida
    INGRESANDO --> VALIDANDO: 4 caracteres

    VALIDANDO --> CONTRASENIA_DINAMICA: entrada = ****
    VALIDANDO --> ACCEDIO: contraseña correcta
    VALIDANDO --> DENEGADO: contraseña incorrecta

    CONTRASENIA_DINAMICA --> ESPERANDO: 4 caracteres nuevos

    ACCEDIO --> ESPERANDO: 3 s
    DENEGADO --> ESPERANDO: 3 s
```

### Estados

- `ESPERANDO`: espera la primera tecla.
- `INGRESANDO`: almacena hasta completar cuatro caracteres.
- `VALIDANDO`: compara los cuatro caracteres ingresados.
- `ACCEDIO`: muestra la figura de acceso.
- `DENEGADO`: muestra una X.
- `CONTRASENIA_DINAMICA`: recibe una nueva contraseña después de ingresar `****`.

## 2. Máquina de estados del antirrebote

```mermaid
stateDiagram-v2
    [*] --> STATE_IDLE

    STATE_IDLE --> STATE_DEBOUNCE_PRESS: tecla detectada
    STATE_DEBOUNCE_PRESS --> STATE_PRESSED: misma tecla durante 20 ms
    STATE_DEBOUNCE_PRESS --> STATE_IDLE: lectura no estable

    STATE_PRESSED --> STATE_DEBOUNCE_RELEASE: tecla liberada
    STATE_DEBOUNCE_RELEASE --> STATE_IDLE: liberada durante 20 ms
    STATE_DEBOUNCE_RELEASE --> STATE_PRESSED: vuelve a detectarse
```

La tecla se entrega a la aplicación únicamente en la transición hacia `STATE_PRESSED`. Por esto una tecla mantenida no genera múltiples eventos.

## 3. Multiplexación de la matriz

```mermaid
flowchart LR
    F0["Fila 0"] --> F1["Fila 1"] --> F2["Fila 2"] --> F3["Fila 3"] --> F4["Fila 4"] --> F5["Fila 5"] --> F6["Fila 6"] --> F7["Fila 7"] --> F0
```

`Matrix_ScanStep()` avanza una sola fila por llamada. El recorrido se repite continuamente desde `main.c`.
