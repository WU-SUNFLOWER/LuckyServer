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
    uint32_t ready_;
    bool in_epoll_;

    std::function<void()> read_callback_;
    std::function<void()> write_callback_;

public:
    explicit Channel(EventLoop *loop, int fd);
    ~Channel();

    void EnableReading();

    void UseET();

    void HandleEvent();

    int GetFd() const;

    uint32_t GetListenEvents() const;

    uint32_t GetReady() const;

    bool GetInEpoll() const;

    void SetInEpoll(bool);

    void SetReady(uint32_t ready);

    void SetReadCallback(std::function<void()> const &callback);

    void SetWriteCallback(std::function<void()> const &callback);
};

#endif // LUCKYSERVER_CHANNEL_H_