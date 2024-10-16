#ifndef HEAD_ACCEPTOR
#define HEAD_ACCEPTOR

#include "event_loop.h"
#include "socket.h"
#include "inet_address.h"
#include "channel.h"
#include <functional>

class Acceptor
{
private:
    EventLoop *loop;
    Socket *socket;
    InetAddress addr;
    Channel *acceptChannel;

    std::function<void(Socket *)> newConnectionCallback;

public:
    Acceptor(EventLoop *_loop);

    ~Acceptor();

    void acceptConnection();

    void setNewConnectionCallBack(std::function<void(Socket *)>);
};

#endif