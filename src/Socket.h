#ifndef HEAD_SOCKET
#define HEAD_SOCKET

#include "InetAddress.h"

class Socket {
private:
    int socket_fd = -1;
public:
    
    Socket();
    
    Socket(int fd);

    ~Socket();

    void close();

    void bind(const InetAddress& addr);

    void listen();

    int accept(InetAddress& clientAddr);

    void setNonBlocking();

    int getFd();
};

#endif