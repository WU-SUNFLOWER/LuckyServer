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
    uint32_t ready;
    bool inEpoll;

    std::function<void()> readCallback;
    std::function<void()> writeCallback;

public:

    Channel(EventLoop* _loop, int _fd);
    ~Channel();

    void enableReading();
    void handleEvent();

    int getFd();

    uint32_t getEvents();

    uint32_t getReady();

    bool getInEpoll();

    void setInEpoll();

    void setReady(uint32_t _revents);

    void setReadCallback(std::function<void()>);

    void setWriteCallback(std::function<void()>);

    void useET();
};

#endif