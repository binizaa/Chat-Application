#include "Client.h"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <ctime>

using namespace std;

Client::Client(int fd) : fd(fd) {
    nick = std::to_string(fd);
}

Client::~Client() {
    close(fd);
}

string Client::generateRandomNick(int length) {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string nick;
    for (int i = 0; i < length; i++) {
        nick += chars[rand() % chars.length()];
    }
    return nick;
}

void Client::queueMessage(const string& message) {
    outgoingQueue.push(message);
}

bool Client::hasDataToSend() const {
    return !currentSendBuffer.empty() || !outgoingQueue.empty();
}

SendResult Client::trySendData() {
    if (currentSendBuffer.empty() && !outgoingQueue.empty()) {
        currentSendBuffer = outgoingQueue.front();
        outgoingQueue.pop();
    }

    if (currentSendBuffer.empty()) {
        return SendResult::COMPLETE;
    }

    ssize_t sent = send(fd, currentSendBuffer.c_str(), currentSendBuffer.size(), 0);
    if (sent < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return SendResult::PENDING;
        }
        cerr << "Socket error on fd " << fd << ": " << strerror(errno) << endl;
        return SendResult::ERROR;
    }

    if (static_cast<size_t>(sent) < currentSendBuffer.size()) {
        currentSendBuffer = currentSendBuffer.substr(sent);
        return SendResult::PENDING;
    }

    currentSendBuffer.clear();
    return outgoingQueue.empty() ? SendResult::COMPLETE : SendResult::PENDING;
}

void Client::setNick(std::string name){
    nick = name;
}