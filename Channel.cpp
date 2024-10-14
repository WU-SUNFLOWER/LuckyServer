#include "Channel.h"
#include "Epoll.h"
#include "util.h"
#include "syscall.h"

Channel::Channel(Epoll* _epoll, int _fd) 
    : epoll(_epoll), fd(_fd), events(0), revents(0), inEpoll(false)
{

}

Channel::~Channel() {

}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    epoll->updateChannel(this);
}

int Channel::getFd() {
    return fd;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getRevents() {
    return revents;
}

bool Channel::getInEpoll() {
    return inEpoll;
}

void Channel::setInEpoll() {
    inEpoll = true;
}

void Channel::setRevents(uint32_t _revents) {
    revents = _revents;
}