# Chat Server (C++ / Kqueue)

Este es un servidor de chat de alto rendimiento y no bloqueante, escrito en C++ moderno y diseñado para sistemas macOS/BSD utilizando `kqueue` para la gestión eficiente de eventos.

## Características

*   **Alto Rendimiento**: Utiliza I/O multiplexado con `kqueue` para manejar múltiples conexiones simultáneamente sin bloquear el hilo principal.
*   **Arquitectura Modular**:
    *   `NetworkServer`: Maneja la creación y configuración de sockets de bajo nivel.
    *   `KqueueEngine`: Abstrae la complejidad de las llamadas al sistema `kevent`.
    *   `ClientManager`: Gestiona la lógica de los clientes y el broadcasting de mensajes.
*   **Gestión de Memoria Moderna**: Uso extensivo de `std::unique_ptr` para la seguridad de memoria y propiedad clara de recursos.

## Requisitos Previos

Este proyecto utiliza la librería [nlohmann/json](https://github.com/nlohmann/json) para el manejo de mensajes.

### Instalación de dependencias

**Opción A: Descarga manual (Recomendado)**
Descarga el archivo `json.hpp` desde el repositorio oficial y colócalo en la siguiente ruta:
`include/external/nlohmann/json.hpp`

**Opción B: Gestores de paquetes**
- **macOS:** `brew install nlohmann-json`
- **Ubuntu/Debian:** `sudo apt-get install nlohmann-json3-dev`
## Estructura del Proyecto

```
server/
├── include/
│   ├── Client.h
│   ├── ClientManager.h
│   └── core/
│       ├── KqueueEngine.h
│       └── NetworkServer.h
└── src/
    ├── Client.cpp
    ├── ClientManager.cpp
    ├── main.cpp
    └── core/
        ├── KqueueEngine.cpp
        └── NetworkServer.cpp
```

## Compilación

Para compilar el proyecto, ejecuta el siguiente comando desde la raíz del proyecto (donde está este README):

```bash
clang++ -std=c++14 \
    server/src/main.cpp \
    server/src/Client.cpp \
    server/src/ClientManager.cpp \
    server/src/core/NetworkServer.cpp \
    server/src/core/KqueueEngine.cpp \
    -I server/include \
    -o chat_server
```

## Ejecución

Una vez compilado, inicia el servidor:

```bash
./chat_server
```

Deberías ver un mensaje indicando que el servidor está corriendo en el puerto `8080`.

## Uso

Para conectarte al servidor y probar el chat, puedes usar `netcat` (`nc`) desde múltiples ventanas de terminal:

```bash
nc localhost 8080
```

Escribe mensajes en una ventana y verás cómo se transmiten a los otros clientes conectados.
