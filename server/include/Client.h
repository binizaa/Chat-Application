#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

enum class SendResult {
    COMPLETE,
    PENDING,
    ERROR
};

class Client {
private:
    int fd;
    std::string name;
    std::string buffer;
    std::queue<std::string> outgoingQueue;
    std::string currentSendBuffer;

public:
    // Constructores
    Client(int fd);
    Client(int fd, std::string name);
    ~Client();

    // Getters
    int getFd() const { return fd; }
    std::string getName() const { return name; }
    std::string getBuffer() const { return buffer; }
    queue<std::string> getOutgoingQueue() const { return outgoingQueue; }
    bool hasDataToSend() const;

    // Setters
    void setName(const string& newName);
    void setBuffer(const std::string& newBuffer);

    // Métodos de lógica
    void queueMessage(const std::string& message);
    SendResult trySendData();
};

#endif