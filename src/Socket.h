#ifndef HEAD_SOCKET
#define HEAD_SOCKET

#include "inet_address.h"

class Socket
{
private:
    int socket_fd_ = -1;

public:
    Socket();

    explicit Socket(int fd);

    ~Socket();

    void close();

    void bind(const InetAddress &kAddr);

    void listen();

    int accept(InetAddress &clientAddr);

    void connect(const InetAddress& addr);

    void setNonBlocking();

    int getFd();
};

#endif