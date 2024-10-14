#ifndef HEAD_SERVER
#define HEAD_SERVER

#include "EventLoop.h"
#include "Socket.h"

class Server {
private:
    EventLoop* loop;
public:
    Server(EventLoop* _loop);
    ~Server();

    void handleReadEvent(int socket_fd);

    void newConnection(Socket* serv_sock);
};

#endif