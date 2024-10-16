#ifndef HEAD_EVENTLOOP
#define HEAD_EVENTLOOP

#include <functional>

class Epoll;
class ThreadPool;
class Channel;

class EventLoop {
private:
    Epoll* epoll_;
    ThreadPool* thread_pool_;
    bool quit_;
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void updateChannel(Channel* channel);

    void addTask(std::function<void()>);
};

#endif