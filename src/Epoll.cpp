#include "epoll.h"
#include "util.h"
#include "syscall.h"

#define MAX_EVENTS 1024

Epoll::Epoll()
{
    epoll_fd = EpollCreate1(0);
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    close(epoll_fd);
    delete[] events;
}

void Epoll::addFd(int fd, uint32_t option)
{
    epoll_event event_config;
    bzero(&event_config, sizeof(event_config));
    event_config.data.fd = fd;
    event_config.events = option;
    EpollCtl(epoll_fd, EPOLL_CTL_ADD, fd, &event_config);
}

std::vector<Channel *> Epoll::wait(int timeout)
{
    std::vector<Channel *> activeChannels;
    int total_fd = EpollWait(epoll_fd, events, MAX_EVENTS, timeout);
    for (int i = 0; i < total_fd; ++i)
    {
        epoll_event &currentEvent = events[i];
        Channel *channel = (Channel *)currentEvent.data.ptr;
        channel->setRevents(currentEvent.events);
        activeChannels.push_back(channel);
    }
    return activeChannels;
}

void Epoll::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    epoll_event event_config;
    bzero(&event_config, sizeof(event_config));

    event_config.data.ptr = channel;
    event_config.events = channel->getEvents();

    if (!channel->getInEpoll())
    {
        EpollCtl(epoll_fd, EPOLL_CTL_ADD, fd, &event_config);
        channel->setInEpoll();
    }
    else
    {
        EpollCtl(epoll_fd, EPOLL_CTL_MOD, fd, &event_config);
    }
}