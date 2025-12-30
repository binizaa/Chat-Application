#include <iostream>
#include <unistd.h>
#include "ClientManager.h"
#include "core/NetworkServer.h"
#include "core/KqueueEngine.h"

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
                        manager.addClient(std::make_unique<Client>(cli_fd));
                        manager.updateClientKqueueEvents(cli_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
                        std::cout << "[SERVER] New connection: " << cli_fd << std::endl;
                    }
                } else if (events[i].filter == EVFILT_READ) {
                    // READ DATA
                    char buf[1024] = {0};
                    ssize_t bytes = read(fd, buf, sizeof(buf) - 1);

                    if (bytes <= 0) {
                        std::cout << "[SERVER] Cliente " << fd << " desconectado." << std::endl;
                        manager.removeClient(fd);
                        close(fd);
                    } else {
                        // --- AQUÍ IMPRIMES EL MENSAJE ---
                        std::string msg(buf);
                        std::cout << "[CLIENTE " << fd << "]: " << msg << std::flush; 
                        
                        // Reenviar a otros (si tu manager lo hace)
                        manager.sendMessage(fd, msg);
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