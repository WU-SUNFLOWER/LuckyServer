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
    // callback_();
    loop_->AddTask(callback_);
}

int Channel::GetFd() const
{
    return fd_;
}

uint32_t Channel::GetEvents() const
{
    return events_;
}

uint32_t Channel::GetRevents() const
{
    return revents_;
}

bool Channel::GetInEpoll() const
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

void Channel::SetCallback(std::function<void()> callback)
{
    callback_ = callback;
}
