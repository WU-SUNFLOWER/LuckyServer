#ifndef LUCKYSERVER_SOCKET_H_
#define LUCKYSERVER_SOCKET_H_

#include "inet_address.h"

class Socket
{
private:
    int socket_fd_ = -1;

public:
    Socket();

    explicit Socket(int fd);

    ~Socket();

    void Close();

    void Bind(const InetAddress &kAddr);

    void Listen();

    int Accept(InetAddress &clientAddr);

    void SetNonBlocking();

    int GetFd() const;
};

#endif // LUCKYSERVER_SOCKET_H_