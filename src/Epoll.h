#ifndef HEAD_EPOLL
#define HEAD_EPOLL

#include <cstdint>
#include <vector>
#include "channel.h"

class Epoll
{
private:
    int epoll_fd_;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void AddFd(int fd, uint32_t option);

    std::vector<Channel *> Wait(int timeout = -1);

    void UpdateChannel(Channel *channel);
};

#endif