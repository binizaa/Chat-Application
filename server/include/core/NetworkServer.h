#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <iostream>

class NetworkServer {
private:
    int serverFd;
    int port;

public:
    NetworkServer(int port);
    ~NetworkServer();

    void start();
    int getFd() const;
};

#endif
