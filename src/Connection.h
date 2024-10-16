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
    std::function<void(Socket*)> DeleteConnectionCallback;

    Buffer* in_buffer_;
    Buffer* read_buffer_;

public:
    Connection(EventLoop* loop, Socket* socket);
    
    ~Connection();

    void Echo(int socket_fd);

    void SetDeleteConnectionCallback(std::function<void(Socket*)>);

};

#endif