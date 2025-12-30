#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include "core/KqueueEngine.h"

class ClientManager {
private:
    std::unordered_map<int, std::unique_ptr<Client>> clients;
    KqueueEngine& engine; 

public:
    ClientManager(KqueueEngine& engine) : engine(engine) {}
    
    ~ClientManager() = default;

    void addClient(std::unique_ptr<Client> client);
    void removeClient(int fd);
    void sendMessage(int senderFd, const std::string& message);
    void updateClientKqueueEvents(int fd, int16_t filter, uint16_t flags);
};

#endif