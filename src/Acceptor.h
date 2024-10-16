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
    EventLoop *loop_;
    Socket *socket_;
    InetAddress address_;
    Channel *accept_channel_;

    std::function<void(Socket *)> NewConnectionCallback;

public:
    Acceptor(EventLoop *loop);

    ~Acceptor();

    void AcceptConnection();

    void SetNewConnectionCallBack(std::function<void(Socket *)>);
};

#endif