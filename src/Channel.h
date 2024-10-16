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

    void enableReading();
    void handleEvent();

    int getFd();

    uint32_t getEvents();

    uint32_t getRevents();

    bool getInEpoll();

    void setInEpoll();

    void setRevents(uint32_t revents);

    void setCallback(std::function<void()>);
};

#endif