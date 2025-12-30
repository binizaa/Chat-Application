#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>

enum class SendResult {
    COMPLETE,
    PENDING,
    ERROR
};

class Client {
public:
    int fd;
    std::string nick;
    std::string buffer;
    std::queue<std::string> outgoingQueue;  // Cola de mensajes salientes
    std::string currentSendBuffer;          // Mensaje actual siendo enviado

    Client(int fd);
    ~Client();

    void queueMessage(const std::string& message);
    bool hasDataToSend() const;
    SendResult trySendData();

    void setNick(std::string name);

private:
    std::string generateRandomNick(int length);
};

#endif