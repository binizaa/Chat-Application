#ifndef KQUEUE_ENGINE_H
#define KQUEUE_ENGINE_H

#include <sys/event.h>
#include <vector>
#include <unistd.h>
#include <stdexcept>

class KqueueEngine {
private:
    int kq;

public:
    KqueueEngine();
    ~KqueueEngine();

    int getKq() const;
    void addEvent(int fd, int16_t filter, uint16_t flags);
    int wait(struct kevent* events, int maxEvents);
};

#endif
