#include "core/NetworkServer.h"

NetworkServer::NetworkServer(int port) : port(port), serverFd(-1) {}

NetworkServer::~NetworkServer() {
    if (serverFd != -1) {
        close(serverFd);
    }
}

void NetworkServer::start() {
    // 1. CREATE SOCKET
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        throw std::runtime_error("Socket creation failed");
    }

    // 2. BIND
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw std::runtime_error("Bind failed");
    }

    // 3. LISTEN
    if (listen(serverFd, SOMAXCONN) == -1) {
        throw std::runtime_error("Listen failed");
    }

    // 4. NON-BLOCKING
    if (fcntl(serverFd, F_SETFL, O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set non-blocking mode");
    }

    std::cout << "Server started on port " << port << std::endl;
}

int NetworkServer::getFd() const {
    return serverFd;
}
