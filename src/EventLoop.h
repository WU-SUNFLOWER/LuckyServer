#ifndef HEAD_EVENTLOOP
#define HEAD_EVENTLOOP

#include <functional>

class Epoll;
class ThreadPool;
class Channel;

class EventLoop {
private:
    Epoll* epoll;
    ThreadPool* threadPool;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void updateChannel(Channel* channel);

    void addTask(std::function<void()>);
};

#endif