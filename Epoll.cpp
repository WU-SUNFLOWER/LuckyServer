#include "Epoll.h"
#include "util.h"
#include "syscall.h"

#define MAX_EVENTS 1024

Epoll::Epoll() {
    epoll_fd = EpollCreate1(0);
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);
}

Epoll::~Epoll() {
    close(epoll_fd);
    delete[] events;
}

void Epoll::addFd(int fd, uint32_t option) {
    struct epoll_event event_config;
    bzero(&event_config, sizeof(event_config));
    event_config.data.fd = fd;
    event_config.events = option;
    EpollCtl(epoll_fd, EPOLL_CTL_ADD, fd, &event_config);
}

std::vector<epoll_event> Epoll::wait() {
    std::vector<epoll_event> activeEvents;
    int total_fd = EpollWait(epoll_fd, events, MAX_EVENTS, -1);
    for (int i = 0; i < total_fd; ++i) {
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}