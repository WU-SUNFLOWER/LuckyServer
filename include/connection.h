#ifndef LUCKYSERVER_CONNECTION_H_
#define LUCKYSERVER_CONNECTION_H_

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection
{
private:
    EventLoop *loop_;
    Socket *socket_;
    Channel *channel_;
    std::function<void(Socket *)> delete_connection_callback_;

    Buffer *in_buffer_;
    Buffer *read_buffer_;

public:
    explicit Connection(EventLoop *loop, Socket *socket);

    ~Connection();

    void Echo();

    void Send();

    void SetDeleteConnectionCallback(std::function<void(Socket *)> const &);
};

#endif // LUCKYSERVER_CONNECTION_H_