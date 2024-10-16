#ifndef HEAD_SOCKET
#define HEAD_SOCKET

#include "inet_address.h"

class Socket
{
private:
    int socket_fd_ = -1;

public:
    Socket();

    Socket(int fd);

    ~Socket();

    void Close();

    void Bind(const InetAddress &kAddr);

    void Listen();

    int Accept(InetAddress &clientAddr);

    void SetNonBlocking();

    int GetFd();
};

#endif