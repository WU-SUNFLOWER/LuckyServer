#ifndef HEAD_CHANNEL
#define HEAD_CHANNEL

#include <cstdint>
#include <functional>

class EventLoop;

class Channel {
private:
    EventLoop* loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;

    std::function<void()> callback;

public:

    Channel(EventLoop* _loop, int _fd);
    ~Channel();

    void enableReading();
    void handleEvent();

    int getFd();

    uint32_t getEvents();

    uint32_t getRevents();

    bool getInEpoll();

    void setInEpoll();

    void setRevents(uint32_t _revents);

    void setCallback(std::function<void()>);
};

#endif