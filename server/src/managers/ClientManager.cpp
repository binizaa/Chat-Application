#include "managers/ClientManager.h"

void ClientManager::addClient(std::unique_ptr<Client> client) {
    int fd = client->getFd();
    clients[fd] = std::move(client);
    std::cout << "Chat conectado: " << clients[fd]->getName() << std::endl;
}

void ClientManager::updateClientKqueueEvents(int fd, int16_t filter, uint16_t flags) {
    engine.addEvent(fd, filter, flags);
}

void ClientManager::sendMessage(int senderFd, const std::string& message) {
    if (clients.find(senderFd) == clients.end()) return;

    std::string senderName = clients[senderFd]->getName();
    std::string fullMessage = senderName + ": " + message + "\r\n";

    for (auto& pair : clients) {
        if (pair.first != senderFd) {
            Client* client = pair.second.get();
            client->queueMessage(fullMessage);

            if (client->getOutgoingQueue().size() == 1) {
                updateClientKqueueEvents(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE);
            }
        }
    }
}

void ClientManager::removeClient(int fd) {
    auto it = clients.find(fd);
    if (it != clients.end()) {
        std::cout << "[CLEANUP] Removing client on FD: " << fd << std::endl;

        // 3. Remove the entry from the unordered_map (unique_ptr handles deletion)
        clients.erase(it);
    }
}

string ClientManager::getNameClient(int fd) {
    return clients[fd]->getName();
}