#include "core/KqueueEngine.h"
#include <iostream>

KqueueEngine::KqueueEngine() {
    kq = kqueue();
    if (kq == -1) {
        throw std::runtime_error("Failed to create kqueue");
    }
}

KqueueEngine::~KqueueEngine() {
    if (kq != -1) {
        close(kq);
    }
}

int KqueueEngine::getKq() const {
    return kq;
}

void KqueueEngine::addEvent(int fd, int16_t filter, uint16_t flags) {
    struct kevent ev;
    EV_SET(&ev, fd, filter, flags, 0, 0, NULL);
    if (kevent(kq, &ev, 1, NULL, 0, NULL) == -1) {
        std::cerr << "Error adding event to kqueue for FD: " << fd << std::endl;
    }
}

int KqueueEngine::wait(struct kevent* events, int maxEvents) {
    return kevent(kq, NULL, 0, events, maxEvents, NULL);
}
