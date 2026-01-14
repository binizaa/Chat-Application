#include "utils/JsonHandler.h"
#include "protocol/Identify.h"
#include <iostream>
#include "external/json.hpp"
#include "utils/constants.h"

using json = nlohmann::json;    

static void sendResponse(int fd, const std::string& operation, const std::string& result, const std::string& extra, ClientManager& manager) {
    protocol::Response response;
    response.operation = operation;
    response.result = result;
    response.extra = extra;

    json j = response;
    manager.sendMessage(fd, j.dump());
}

static void sendProtocolError(int fd, ClientManager& manager) {
    json response = {
        {"type", "INVALID"},
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
        auto dto = j.get<protocol::Identify>();

        std::cout << "[IDENTIFY] Client on FD " << fd << " attempting to identify as: " << dto.username << std::endl;

        // Delegamos la validación al manager
        auto result = manager.registerUser(fd, dto.username);

        switch (result) {
            case RegistrationResult::SUCCESS: {
                std::cout << "[SUCCESS] " << dto.username << " has successfully identified" << std::endl;

                // Respuesta al cliente que se identificó
                sendResponse(fd, "IDENTIFY", "SUCCESS", dto.username, manager);

                // Broadcast NEW_USER a los demás clientes
                protocol::NewUser newUserMsg;
                newUserMsg.username = dto.username;
                json broadcast = newUserMsg;
                manager.broadcastNewUser(fd, broadcast.dump());

                std::cout << "[BROADCAST] Notifying other clients about new user: " << dto.username << std::endl;
                break;
            }

            case RegistrationResult::ALREADY_EXISTS:
                std::cout << "[ERROR] Username already exists: " << dto.username << std::endl;
                sendResponse(fd, "IDENTIFY", "USER_ALREADY_EXISTS", dto.username, manager);
                break;

            case RegistrationResult::INVALID_FORMAT:
                std::cout << "[ERROR] Invalid username format: " << dto.username << std::endl;
                sendResponse(fd, "IDENTIFY", "INVALID", dto.username, manager);
                manager.disconnectClient(fd);
                break;

            case RegistrationResult::NOT_IDENTIFIED:
                std::cout << "[ERROR] Client not found: " << dto.username << std::endl;
                sendResponse(fd, "IDENTIFY", "NOT_IDENTIFIED", dto.username, manager);
                break;
        }

    } catch (const std::exception& e) {
        std::cout << "[ERROR] Failed to parse IDENTIFY message from FD " << fd << ": " << e.what() << std::endl;
        sendProtocolError(fd, manager);
        manager.disconnectClient(fd);
    }
}
