#ifndef HEAD_CHANNEL
#define HEAD_CHANNEL

#include <cstdint>
#include <functional>

class EventLoop;

class Channel {
private:
    EventLoop* loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    bool in_epoll_;

    std::function<void()> callback;

public:

    Channel(EventLoop* loop, int fd);
    ~Channel();

    void EnableReading();
    void HandleEvent();

    int GetFd();

    uint32_t GetEvents();

    uint32_t GetRevents();

    bool GetInEpoll();

    void SetInEpoll();

    void SetRevents(uint32_t revents);

    void SetCallback(std::function<void()>);
};

#endif