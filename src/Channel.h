#ifndef LUCKYSERVER_CHANNEL_H_
#define LUCKYSERVER_CHANNEL_H_

#include <cstdint>
#include <functional>

class EventLoop;

class Channel
{
private:
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    bool in_epoll_;

    std::function<void()> callback_;

public:
    explicit Channel(EventLoop *loop, int fd);
    ~Channel();

    void EnableReading();
    void HandleEvent();

    int GetFd() const;

    uint32_t GetEvents() const;

    uint32_t GetRevents() const;

    bool GetInEpoll() const;

    void SetInEpoll();

    void SetRevents(uint32_t revents);

    void SetCallback(const std::function<void()>);
};

#endif // LUCKYSERVER_CHANNEL_H_