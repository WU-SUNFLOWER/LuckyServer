#ifndef HEAD_INETADDRESS
#define HEAD_INETADDRESS

#include "syscall.h"
#include "util.h"
#include <string>

class InetAddress {
private:
    struct sockaddr_in sockaddr;
    socklen_t sockaddr_len_;
public:

    InetAddress();

    InetAddress(const std::string& kAddr, uint16_t port);
    
    ~InetAddress();

    const struct sockaddr_in* getSockAddr() const {
        return &sockaddr;
    }

    socklen_t getSockLen() const {
        return sockaddr_len_;
    }

    std::string getIP() const {
        return inet_ntoa(sockaddr.sin_addr);
    }

    uint16_t getPort() {
        return ntohs(sockaddr.sin_port);
    }

    void setInetAddr(struct sockaddr_in& sockaddr);
};

#endif