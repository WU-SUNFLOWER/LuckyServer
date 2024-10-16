#include "channel.h"
#include "event_loop.h"
#include "util.h"
#include "syscall.h"

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), in_epoll_(false)
{
}

Channel::~Channel()
{
}

void Channel::EnableReading()
{
    events_ = EPOLLIN | EPOLLET;
    loop_->UpdateChannel(this);
}

void Channel::HandleEvent()
{
    // callback();
    loop_->AddTask(callback);
}

int Channel::GetFd()
{
    return fd_;
}

uint32_t Channel::GetEvents()
{
    return events_;
}

uint32_t Channel::GetRevents()
{
    return revents_;
}

bool Channel::GetInEpoll()
{
    return in_epoll_;
}

void Channel::SetInEpoll()
{
    in_epoll_ = true;
}

void Channel::SetRevents(uint32_t _revents)
{
    revents_ = _revents;
}

void Channel::SetCallback(std::function<void()> _callback)
{
    callback = _callback;
}
