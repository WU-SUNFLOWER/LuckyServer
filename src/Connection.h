#ifndef HEAD_CONNECTION
#define HEAD_CONNECTION

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
private:
    EventLoop* loop_;
    Socket* socket_;
    Channel* channel_;
    std::function<void(Socket*)> deleteConnectionCallback;

    Buffer* in_buffer_;
    Buffer* read_buffer_;

public:
    Connection(EventLoop* loop, Socket* socket);
    
    ~Connection();

    void echo();
    void send();

    void setDeleteConnectionCallback(std::function<void(Socket*)>);

};

#endif