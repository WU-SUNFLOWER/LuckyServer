#ifndef LUCKYSERVER_INETADDRESS_H_
#define LUCKYSERVER_INETADDRESS_H_

#include <string>

#include "syscall.h"
#include "util.h"

class InetAddress
{
private:
    struct sockaddr_in sockaddr;
    socklen_t sockaddr_len_;

public:
    InetAddress();

    explicit InetAddress(const std::string &kAddr, uint16_t port);

    ~InetAddress();

    const struct sockaddr_in *GetSockAddress() const
    {
        return &sockaddr;
    }

    socklen_t GetSockLen() const
    {
        return sockaddr_len_;
    }

    std::string GetIP() const
    {
        return inet_ntoa(sockaddr.sin_addr);
    }

    uint16_t GetPort()
    {
        return ntohs(sockaddr.sin_port);
    }

    void SetInetAddress(struct sockaddr_in &sockaddr);
};

#endif // LUCKYSERVER_INETADDRESS_H_