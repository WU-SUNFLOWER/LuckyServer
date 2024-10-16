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
    ThreadPool *thread_pool_;
    bool quit_;

public:
    EventLoop();
    ~EventLoop();

    void Loop();

    void UpdateChannel(Channel *channel);

    void AddTask(std::function<void()>);
};

#endif // LUCKYSERVER_EVENTLOOP_H_