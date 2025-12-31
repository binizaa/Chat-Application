#include "utils/JsonHandler.h"
#include "models/DTO/UserDTO.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include "utils/constants.h"

using json = nlohmann::json;    

static void sendResponse(int fd, const std::string& type, const std::string& status, const std::string& username, ClientManager& manager) {
    json response = {
        {"type", type},
        {"status", status},
        {"username", username}
    };
    manager.sendMessage(fd, response.dump());
}

static void sendProtocolError(int fd, const std::string& type, ClientManager& manager) {
    json response = {
        {"type", type},
        {"message", "Protocol Error"}
    };
    manager.sendMessage(fd, response.dump());
}

void JsonHandler::HandleBuffer(int fd, const std::string& buffer, ClientManager& manager) {
    try {
        if (buffer.empty()) return;
        
        auto j = json::parse(buffer);
        
        if (!j.contains("type")) {
             json response = { {"type", "INVALID"}, {"message", "Missing 'type' field"} };
             manager.sendMessage(fd, response.dump());
             return;
        }
        
        std::string type = j["type"];
        
        if (type == "IDENTIFY") {
            handleIdentify(fd, buffer, manager);
        }
        else {
             // For now, handling generic messages or unknown types
             // Echoing for now to match previous behavior if not identify?
             // Or better, send UNKNOWN_COMMAND
             // But for "chat", we might not have a handler yet.
             // Let's print to console at least.
             std::string name = manager.getNameClient(fd);
             std::cout << "[LOG] " << name << " sent type: " << type << std::endl;
             
             // If we want to keep previous echo behavior for non-commands (as a fallback chat)
             // we'd need to know if it's a CHAT type.
             // Assuming strict JSON protocol now.
             
             json response = { {"type", "UNKNOWN"}, {"message", "Unknown command type"} };
             manager.sendMessage(fd, response.dump());
        }

    } catch (const json::parse_error& e) {
        // Not JSON? Maybe treat as legacy chat message?
        // Or return error. 
        // User asked "modifica el main.cpp para que funcione" with handleIdentify.
        // I will return INVALID JSON to enforce protocol.
        json response = {
            {"type", "INVALID"},
            {"message", "Invalid JSON format"}
        };
        manager.sendMessage(fd, response.dump());
    } catch (const std::exception& e) {
         std::cerr << "Error handling buffer: " << e.what() << std::endl;
    }
}

void JsonHandler::handleIdentify(int fd, const std::string& jsonString, ClientManager& manager) {
    try {
        auto j = nlohmann::json::parse(jsonString);
        auto dto = j.get<UserDTO::IDENTIFY>();

        // Delegamos la validación al manager
        auto result = manager.registerUser(fd, dto.username);

        switch (result) {
            case RegistrationResult::SUCCESS:
                sendResponse(fd, "IDENTIFY", "SUCCESS", dto.username, manager);
                // Notificamos a los demás
                manager.broadcastNewUser(fd, dto.username);
                break;

            case RegistrationResult::ALREADY_EXISTS:
                sendResponse(fd, "IDENTIFY", "USER_ALREADY_EXISTS", dto.username, manager);
                break;

            case RegistrationResult::INVALID_FORMAT:
                sendResponse(fd, "IDENTIFY", "INVALID", dto.username, manager);
                manager.disconnectClient(fd); // Regla: Desconectar por datos inválidos
                break;

            case RegistrationResult::NOT_IDENTIFIED:
                sendResponse(fd, "IDENTIFY", "NOT_IDENTIFIED", dto.username, manager);
                break;
        }

    } catch (const std::exception& e) {
        sendProtocolError(fd, "INVALID", manager);
        manager.disconnectClient(fd);
    }
}
