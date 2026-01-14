#include "managers/ClientManager.h"

using namespace std;




RegistrationResult ClientManager::registerUser(int fd, const std::string& username) {
    // 1. Regla de longitud (máx 8 caracteres)
    if (username.empty()) {
        return RegistrationResult::INVALID_FORMAT;
    }

    // 2. Regla de unicidad
    if (isNameTaken(username)) {
        return RegistrationResult::ALREADY_EXISTS;
    }

    // 3. Éxito: Actualizar estado del cliente
    // Ensure client exists
    if (clients.find(fd) == clients.end()) {
        return RegistrationResult::NOT_IDENTIFIED; // Should verify if client exists
    }
    
    auto& client = clients[fd]; 
    client->setName(username);
    client->setIdentified(true);
    
    return RegistrationResult::SUCCESS;
}

bool ClientManager::addClient(std::unique_ptr<Client> client) {
    if (!client) return false;
    clients[client->getFd()] = std::move(client);
    return true;
}

bool ClientManager::isNameTaken(const std::string& name) {
    for (const auto& pair : clients) {
        if (pair.second->getName() == name) {
            return true;
        }
    }
    return false;
}

void ClientManager::setClientName(int fd, const std::string& name) {
    if (clients.find(fd) != clients.end()) {
        clients[fd]->setName(name);
    }
}

void ClientManager::updateClientKqueueEvents(int fd, int16_t filter, uint16_t flags) {
    engine.addEvent(fd, filter, flags);
}


void ClientManager::sendMessage(int fd, const string& message) {
    if (clients.find(fd) == clients.end()) return;

    Client* client = clients[fd].get();
    client->queueMessage(message);

    if (client->getOutgoingQueue().size() == 1) {
        updateClientKqueueEvents(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE);
    }
}

void ClientManager::broadcastNewUser(int fd, const std::string& message) {
    for (auto& pair : clients) {
        if (pair.first != fd) {
             Client* client = pair.second.get();
             client->queueMessage(message);

             if (client->getOutgoingQueue().size() == 1) {
                 updateClientKqueueEvents(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE);
             }
        }
    }
}

void ClientManager::handleClientWrite(int fd) {
    if (clients.find(fd) == clients.end()) return;

    Client* client = clients[fd].get();
    SendResult result = client->trySendData();

    if (result == SendResult::COMPLETE || result == SendResult::ERROR) {
        // Disable write events if no more data to send or error occurred
        if (!client->hasDataToSend()) {
            updateClientKqueueEvents(fd, EVFILT_WRITE, EV_DELETE);
        }
    }
}

void ClientManager::removeClient(int fd) {
    auto it = clients.find(fd);
    if (it != clients.end()) {
        cout << "[CLEANUP] Removing client on FD: " << fd << endl;

        // 3. Remove the entry from the unordered_map (unique_ptr handles deletion)
        clients.erase(it);
    }
}

string ClientManager::getNameClient(int fd) {
    return clients[fd]->getName();
}