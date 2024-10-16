#include "event_loop.h"

#include <vector>

#include "epoll.h"
#include "channel.h"
#include "thread_pool.h"

EventLoop::EventLoop()
    : epoll_(nullptr), quit_(false)
{
    epoll_ = new Epoll();
}

EventLoop::~EventLoop()
{
    delete epoll_;
}

void EventLoop::Loop()
{
    while (!quit_)
    {
        std::vector<Channel *> channels = epoll_->Wait();
        for (Channel *channel : channels)
        {
            channel->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel *channel)
{
    epoll_->UpdateChannel(channel);
}
