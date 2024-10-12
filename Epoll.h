#ifndef HEAD_EPOLL
#define HEAD_EPOLL

#include <cstdint>
#include <vector>

class Epoll {
private:
    int epoll_fd;
    struct epoll_event* events;
public:
    Epoll();
    ~Epoll();
    void addFd(int fd, uint32_t option);
    std::vector<epoll_event> wait();
};

#endif