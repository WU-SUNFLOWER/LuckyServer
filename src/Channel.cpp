#include "Channel.h"
#include "EventLoop.h"
#include "util.h"
#include "syscall.h"

Channel::Channel(EventLoop* _loop, int _fd) 
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false)
{

}

Channel::~Channel() {
    
}

void Channel::enableReading() {
    events |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

void Channel::useET() {
    events |= EPOLLET;
    loop->updateChannel(this);
}

void Channel::handleEvent() {
    if (ready & (EPOLLIN | EPOLLPRI)) {
        readCallback();
    }
    if (ready & EPOLLOUT) {
        writeCallback();
    }
}

int Channel::getFd() {
    return fd;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getReady() {
    return ready;
}

bool Channel::getInEpoll() {
    return inEpoll;
}

void Channel::setInEpoll() {
    inEpoll = true;
}

void Channel::setReady(uint32_t _revents) {
    ready = _revents;
}

void Channel::setReadCallback(std::function<void()> _callback) {
    readCallback = _callback;
}

void Channel::setWriteCallback(std::function<void()> _callback) {
    writeCallback = _callback;
}