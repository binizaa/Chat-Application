#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <string>
#include "managers/ClientManager.h"
#include "external/json.hpp"

// JsonHandler.h: Esta es la capa más crítica. Recibe el string del socket y determina el tipo de mensaje.
// Si falta una llave o el valor es inválido (ej. un estado que no sea AWAY): Construye el JSON de INVALID y marca al cliente para desconexión.
// Si el cliente no se ha identificado: Bloquea cualquier comando que no sea IDENTIFY y devuelve NOT_IDENTIFIED.

class JsonHandler {
public:
    static void handleIdentify(int fd, const std::string& jsonString, ClientManager& manager);
    static void HandleBuffer(int fd, const std::string& buffer, ClientManager& manager);
};

#endif
