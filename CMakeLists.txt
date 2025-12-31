cmake_minimum_required(VERSION 3.14)
project(ChatServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorios de inclusión (Asegúrate de incluir todos los niveles)
include_directories(
    server/include
    server/include/core
    server/include/models
    server/include/models/DTO
    server/include/external
)

# Lista de archivos fuente con RUTAS CORRECTAS
set(SOURCES
    server/src/main.cpp
    server/src/core/Chat.cpp           # Agregado
    server/src/core/Client.cpp         # Corregido: dentro de core
    server/src/core/ClientManager.cpp  # Corregido: dentro de core
    server/src/core/NetworkServer.cpp
    server/src/core/KqueueEngine.cpp
    # server/src/utils/JsonHandler.cpp # Agrégalo cuando lo crees
)

add_executable(chat_server ${SOURCES})

# Configuración de nlohmann_json
target_include_directories(chat_server PRIVATE server/include/external)