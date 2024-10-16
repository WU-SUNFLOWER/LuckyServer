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

    uint32_t GetEvents() const;

    uint32_t GetReady() const;

    bool GetInEpoll() const;

    void SetInEpoll();

    void SetReady(uint32_t ready);

    void SetReadCallback(std::function<void()> callback);

    void SetWriteCallback(std::function<void()> callback);
};

#endif // LUCKYSERVER_CHANNEL_H_