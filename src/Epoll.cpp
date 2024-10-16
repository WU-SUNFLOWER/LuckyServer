#include "epoll.h"

#include "util.h"
#include "syscall.h"

#define MAX_EVENTS 1024

Epoll::Epoll() : events(new epoll_event[MAX_EVENTS])
{
    epoll_fd_ = mysyscall::EpollCreate1(0);
}

Epoll::~Epoll()
{
    mysyscall::Close(epoll_fd_);
    delete[] events;
}

void Epoll::AddFd(int fd, uint32_t option)
{
    epoll_event event_config{};
    event_config.data.fd = fd;
    event_config.events = option;
    mysyscall::EpollCtl(epoll_fd_, EPOLL_CTL_ADD, fd, &event_config);
}

std::vector<Channel *> Epoll::Wait(int timeout)
{
    std::vector<Channel *> active_channels;
    int total_fd = mysyscall::EpollWait(epoll_fd_, events, MAX_EVENTS, timeout);
    for (int i = 0; i < total_fd; ++i)
    {
        epoll_event &currentEvent = events[i];
        Channel *channel = static_cast<Channel *>(currentEvent.data.ptr);
        channel->SetReady(currentEvent.events);
        active_channels.push_back(channel);
    }
    return active_channels;
}

void Epoll::UpdateChannel(Channel *channel)
{
    int fd = channel->GetFd();
    epoll_event event_config{};

    event_config.data.ptr = channel;
    event_config.events = channel->GetEvents();

    if (!channel->GetInEpoll())
    {
        mysyscall::EpollCtl(epoll_fd_, EPOLL_CTL_ADD, fd, &event_config);
        channel->SetInEpoll(true);
    }
    else
    {
        mysyscall::EpollCtl(epoll_fd_, EPOLL_CTL_MOD, fd, &event_config);
    }
}

void Epoll::DeleteChannel(Channel *channel)
{
    int channel_fd = channel->GetFd();
    util::ErrIf(channel_fd == -1, "delete channel error");

    if (channel->GetInEpoll()) {
        channel->SetInEpoll(false);
        mysyscall::EpollCtl(epoll_fd_, EPOLL_CTL_DEL, channel_fd, nullptr);
    }
}
