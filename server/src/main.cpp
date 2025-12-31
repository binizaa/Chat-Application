#include <iostream>
#include <unistd.h>
#include "ClientManager.h"
#include "core/NetworkServer.h"
#include "core/KqueueEngine.h"


using namespace std;

int main() {
    try {
        // 1. Initialize Core Components
        KqueueEngine engine;
        ClientManager manager(engine);
        NetworkServer server(8080);

        server.start();

        // 2. Register Server Socket
        engine.addEvent(server.getFd(), EVFILT_READ, EV_ADD | EV_ENABLE);

        std::cout << "Chat Server running..." << std::endl;

        // 3. Event Loop
        struct kevent events[32];
        while (true) {
            int n = engine.wait(events, 32);
            
            for (int i = 0; i < n; i++) {
                int fd = events[i].ident;

                if (fd == server.getFd()) {
                    // NEW CONNECTION
                    struct sockaddr_in clientAddr;
                    socklen_t clientLen = sizeof(clientAddr);
                    int cli_fd = accept(server.getFd(), (struct sockaddr*)&clientAddr, &clientLen);
                    
                    if (cli_fd != -1) {
                        fcntl(cli_fd, F_SETFL, O_NONBLOCK);

                        auto newCliente = std::make_unique<Client>(cli_fd, "Name");

                        manager.addClient(std::move(newCliente));
                        manager.updateClientKqueueEvents(cli_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
                        std::cout << "[SERVER] New connection: " << cli_fd << std::endl;
                    }
                } else if (events[i].filter == EVFILT_READ) {
                    char buf[1024];
                    ssize_t bytes = read(fd, buf, sizeof(buf) - 1);

                    if (bytes <= 0) {
                        std::cout << "[SERVER] Cliente " << fd << " desconectado por cierre de socket." << std::endl;
                        manager.removeClient(fd);
                        close(fd); 
                    } else {
                        buf[bytes] = '\0'; 
                        string msg(buf);

                        msg.erase(msg.find_last_not_of("\n\r") + 1);

                        if (msg == "exit") {
                            std::cout << "[SERVER] Cliente " << fd << " solicitó salida." << std::endl;
                            manager.removeClient(fd);
                            close(fd);
                        } else {

                            string name = manager.getNameClient(fd);

                            std::cout << "[LOG] " << name << ": " << msg << std::endl;
                            manager.sendMessage(fd, msg);
                        }
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}