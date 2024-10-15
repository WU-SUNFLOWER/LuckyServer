#ifndef HEAD_CONNECTION
#define HEAD_CONNECTION

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Connection;

class Connection {
private:
    EventLoop* loop;
    Socket* socket;
    Channel* channel;
    std::function<void(Socket*)> deleteConnectionCallback;
public:
    Connection(EventLoop* _loop, Socket* _socket);
    
    ~Connection();

    void echo(int socket_fd);

    void setDeleteConnectionCallback(std::function<void(Socket*)>);

};

#endif