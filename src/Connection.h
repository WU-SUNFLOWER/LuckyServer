#ifndef HEAD_CONNECTION
#define HEAD_CONNECTION

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
private:
    EventLoop* loop;
    Socket* socket;
    Channel* channel;
    std::function<void(Socket*)> deleteConnectionCallback;

    Buffer* inBuffer;
    Buffer* readBuffer;

public:
    Connection(EventLoop* _loop, Socket* _socket);
    
    ~Connection();

    void echo();
    void send();

    void setDeleteConnectionCallback(std::function<void(Socket*)>);

};

#endif