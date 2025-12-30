#include "ClientManager.h"

void ClientManager::addClient(std::unique_ptr<Client> client) {
    int fd = client->fd;
    clients[fd] = std::move(client);
    std::cout << "Chat conectado: " << clients[fd]->nick << std::endl;
}

void ClientManager::updateClientKqueueEvents(int fd, int16_t filter, uint16_t flags) {
    engine.addEvent(fd, filter, flags);
}

void ClientManager::sendMessage(int senderFd, const std::string& message) {
    if (clients.find(senderFd) == clients.end()) return;

    std::string senderNick = clients[senderFd]->nick;
    std::string fullMessage = senderNick + ": " + message + "\r\n";

    for (auto& pair : clients) {
        if (pair.first != senderFd) {
            Client* client = pair.second.get();
            client->queueMessage(fullMessage);

            if (client->outgoingQueue.size() == 1) {
                updateClientKqueueEvents(client->fd, EVFILT_WRITE, EV_ADD | EV_ENABLE);
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