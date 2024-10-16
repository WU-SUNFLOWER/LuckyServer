#include "event_loop.h"
#include "epoll.h"
#include "channel.h"
#include "thread_pool.h"
#include <vector>

EventLoop::EventLoop()
    : epoll_(nullptr), thread_pool_(nullptr), quit_(false)
{
    epoll_ = new Epoll();
    thread_pool_ = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete epoll_;
    delete thread_pool_;
}

void EventLoop::loop()
{
    while (!quit_)
    {
        std::vector<Channel *> channels = epoll_->wait();
        for (Channel *channel : channels)
        {
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    epoll_->updateChannel(channel);
}

void EventLoop::addTask(std::function<void()> func)
{
    thread_pool_->addTask(func);
}
