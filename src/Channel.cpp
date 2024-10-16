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

void Channel::enableReading()
{
    events_ = EPOLLIN | EPOLLET;
    loop_->updateChannel(this);
}

void Channel::handleEvent()
{
    // callback();
    loop_->addTask(callback);
}

int Channel::getFd()
{
    return fd_;
}

uint32_t Channel::getEvents()
{
    return events_;
}

uint32_t Channel::getRevents()
{
    return revents_;
}

bool Channel::getInEpoll()
{
    return in_epoll_;
}

void Channel::setInEpoll()
{
    in_epoll_ = true;
}

void Channel::setRevents(uint32_t _revents)
{
    revents_ = _revents;
}

void Channel::setCallback(std::function<void()> _callback)
{
    callback = _callback;
}
