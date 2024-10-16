#ifndef HEAD_SERVER
#define HEAD_SERVER

#include <map>

class EventLoop;
class Acceptor;
class Socket;
class Connection;

class Server {
private:
    EventLoop* loop_;
    Acceptor* acceptor_;
    std::map<int, Connection*> connections_;
public:
    Server(EventLoop* loop);
    
    ~Server();

    void newConnection(Socket* serv_sock);

    void deleteConnection(Socket*);
};

#endif