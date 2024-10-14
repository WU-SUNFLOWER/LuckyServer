#ifndef HEAD_EVENTLOOP
#define HEAD_EVENTLOOP

#include "Epoll.h"
#include "Channel.h"

class EventLoop {
private:
    Epoll* epoll;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void updateChannel(Channel* channel);
};

#endif