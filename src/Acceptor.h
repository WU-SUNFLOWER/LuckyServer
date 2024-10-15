#ifndef HEAD_ACCEPTOR
#define HEAD_ACCEPTOR

#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>

class Acceptor {
private:
    EventLoop* loop;
    Socket* socket;
    InetAddress addr;
    Channel* acceptChannel;

    std::function<void(Socket*)> newConnectionCallback;

public:
    Acceptor(EventLoop* _loop);
    
    ~Acceptor();

    void acceptConnection();

    void setNewConnectionCallBack(std::function<void(Socket*)>);
};

#endif