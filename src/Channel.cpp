#include "channel.h"
#include "event_loop.h"
#include "util.h"
#include "syscall.h"

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), ready_(0), in_epoll_(false)
{
}

Channel::~Channel()
{
}

void Channel::EnableReading()
{
    events_ |= EPOLLIN | EPOLLPRI;
    loop_->UpdateChannel(this);
}
void Channel::UseET()
{
    events_ |= EPOLLET;
    loop_->UpdateChannel(this);
}

void Channel::HandleEvent()
{
    if (ready_ & (EPOLLIN | EPOLLPRI))
    {
        read_callback_();
    }
    if (ready_ & EPOLLOUT)
    {
        write_callback_();
    }
}

int Channel::GetFd() const
{
    return fd_;
}

uint32_t Channel::GetEvents() const
{
    return events_;
}

uint32_t Channel::GetReady() const
{
    return ready_;
}

bool Channel::GetInEpoll() const
{
    return in_epoll_;
}

void Channel::SetInEpoll()
{
    in_epoll_ = true;
}

void Channel::SetReady(uint32_t ready)
{
    ready_ = ready;
}

void Channel::SetReadCallback(std::function<void()> callback)
{
    read_callback_ = callback;
}

void Channel::SetWriteCallback(std::function<void()> callback)
{
    write_callback_ = callback;
}