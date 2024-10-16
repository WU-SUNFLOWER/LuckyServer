#include "event_loop.h"
#include "epoll.h"
#include "channel.h"
#include "thread_pool.h"
#include <vector>

EventLoop::EventLoop()
    : epoll(nullptr), threadPool(nullptr), quit(false)
{
    epoll = new Epoll();
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete epoll;
    delete threadPool;
}

void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<Channel *> channels = epoll->wait();
        for (Channel *channel : channels)
        {
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    epoll->updateChannel(channel);
}

void EventLoop::addTask(std::function<void()> func)
{
    threadPool->addTask(func);
}
