#ifndef LUCKYSERVER_EVENTLOOP_H_
#define LUCKYSERVER_EVENTLOOP_H_

#include <functional>
class Epoll;
class ThreadPool;
class Channel;

class EventLoop
{
private:
    Epoll *epoll_;
    bool quit_;

public:
    EventLoop();
    ~EventLoop();

    void Loop();

    void UpdateChannel(Channel *channel);

    void DeleteChannel(Channel *channel);
};

#endif // LUCKYSERVER_EVENTLOOP_H_