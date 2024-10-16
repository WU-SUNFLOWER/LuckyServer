#ifndef LUCKYSERVER_INETADDRESS_H_
#define LUCKYSERVER_INETADDRESS_H_

#include <string>

#include "syscall.h"
#include "util.h"

class InetAddress
{
private:
    sockaddr_in sockaddr_;
    socklen_t sockaddr_len_;

public:
    InetAddress();

    explicit InetAddress(const std::string &kAddr, uint16_t port);

    ~InetAddress();

    const sockaddr_in *GetSockAddress() const
    {
        return &sockaddr_;
    }

    socklen_t GetSockLen() const
    {
        return sockaddr_len_;
    }

    std::string GetIP() const
    {
        return inet_ntoa(sockaddr_.sin_addr);
    }

    uint16_t GetPort()
    {
        return ntohs(sockaddr_.sin_port);
    }

    void SetInetAddress(struct sockaddr_in &sockaddr_);
};

#endif // LUCKYSERVER_INETADDRESS_H_