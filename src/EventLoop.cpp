#include "EventLoop.h"
#include <vector>

EventLoop::EventLoop()
 : epoll(nullptr), quit(false) 
{
    epoll = new Epoll();
}

EventLoop::~EventLoop() {
    delete epoll;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> channels = epoll->wait();
        for (Channel* channel: channels) {
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel) {
    epoll->updateChannel(channel);
}
