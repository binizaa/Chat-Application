#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "models/Client.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include "core/KqueueEngine.h"
#include "utils/constants.h"

class ClientManager {
private:
    std::unordered_map<int, std::unique_ptr<Client>> clients;
    KqueueEngine& engine; 

public:
    ClientManager(KqueueEngine& engine) : engine(engine) {}
    
    ~ClientManager() = default;

    // Modificado para coincidir con la implementación y requerimientos
    bool addClient(std::unique_ptr<Client> client);
    RegistrationResult registerUser(int fd, const std::string& username);
    
    void removeClient(int fd);
    // Helper alias for clarity if needed, or just use removeClient
    void disconnectClient(int fd) { removeClient(fd); }

    void sendMessage(int senderFd, const std::string& message);
    void broadcastNewUser(int fd, const std::string& username);

    void updateClientKqueueEvents(int fd, int16_t filter, uint16_t flags);

    bool isNameTaken(const std::string& name);
    void setClientName(int fd, const std::string& name);
    std::string getNameClient(int fd);
};

#endif