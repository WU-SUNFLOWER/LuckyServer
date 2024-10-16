#ifndef LUCKYSERVER_ACCEPTOR_H_
#define LUCKYSERVER_ACCEPTOR_H_

#include <functional>

#include "event_loop.h"
#include "socket.h"
#include "inet_address.h"
#include "channel.h"

class Acceptor
{

private:
    EventLoop *loop_;
    Socket *socket_;
    InetAddress address_;
    Channel *accept_channel_;

    std::function<void(Socket *)> new_connection_callback_;

public:
    explicit Acceptor(EventLoop *loop);

    ~Acceptor();

    void AcceptConnection();

    void SetNewConnectionCallBack(std::function<void(Socket *)>);
};

#endif // LUCKYSERVER_ACCEPTOR_H_