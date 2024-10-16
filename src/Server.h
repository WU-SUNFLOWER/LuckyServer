#ifndef HEAD_SERVER
#define HEAD_SERVER

#include <map>
#include <vector>

class EventLoop;
class Acceptor;
class Socket;
class Connection;
class ThreadPool;

class Server {
private:
    EventLoop* mainReactor;

    Acceptor* acceptor;
    
    std::map<int, Connection*> connections;

    std::vector<EventLoop*> subReactors;

    ThreadPool* threadPool;
public:
    Server(EventLoop* _loop);
    
    ~Server();

    void handleReadEvent(int);

    void newConnection(Socket* serv_sock);

    void deleteConnection(Socket*);
};

#endif