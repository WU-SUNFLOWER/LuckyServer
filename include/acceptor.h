#ifndef LUCKYSERVER_ACCEPTOR_H_
#define LUCKYSERVER_ACCEPTOR_H_

#include <functional>

#include "inet_address.h"

class EventLoop;
class Socket;
class Channel;

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

    void SetNewConnectionCallBack(std::function<void(Socket *)> const &);
};

#endif // LUCKYSERVER_ACCEPTOR_H_