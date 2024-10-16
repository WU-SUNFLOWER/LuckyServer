#ifndef LUCKYSERVER_SERVER_H_
#define LUCKYSERVER_SERVER_H_

#include <map>
#include <vector>

class EventLoop;
class Acceptor;
class Socket;
class Connection;
class ThreadPool;

class Server
{
private:
    EventLoop *main_reactor_;
    Acceptor *acceptor_;
    std::map<int, Connection *> connections_;
    std::vector<EventLoop *> sub_reactors_;
    ThreadPool *thread_pool_;

public:
    explicit Server(EventLoop *loop);

    ~Server();

    void HandleReadEvent(int);

    void NewConnection(Socket *serv_sock);

    void DeleteConnection(Socket *socket);
};

#endif // LUCKYSERVER_SERVER_H_