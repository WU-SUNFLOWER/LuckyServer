#ifndef LUCKYSERVER_ACCEPTOR_H_
#define LUCKYSERVER_ACCEPTOR_H_

class Server;
class EventLoop;
class Connection;

class HttpServer 
{
private:
    EventLoop *loop_;
    Server *server_;

public:
    explicit HttpServer();

    ~HttpServer();

    void Run();

    void OnConnect(Connection *);
};

#endif