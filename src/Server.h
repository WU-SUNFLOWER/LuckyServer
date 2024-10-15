#ifndef HEAD_SERVER
#define HEAD_SERVER

#include <map>

class EventLoop;
class Acceptor;
class Socket;
class Connection;

class Server {
private:
    EventLoop* loop;
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
public:
    Server(EventLoop* _loop);
    
    ~Server();

    void newConnection(Socket* serv_sock);

    void deleteConnection(Socket*);
};

#endif