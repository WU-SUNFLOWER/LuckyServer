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

    void addFd(int fd, uint32_t option);

    std::vector<Channel *> wait(int timeout = -1);

    void updateChannel(Channel *channel);
};

#endif