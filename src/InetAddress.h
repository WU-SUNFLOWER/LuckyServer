#ifndef HEAD_INETADDRESS
#define HEAD_INETADDRESS

#include "syscall.h"
#include "util.h"
#include <string>

class InetAddress {
private:
    sockaddr_in sockaddr;
    socklen_t sockaddr_len;
public:

    InetAddress();

    InetAddress(const std::string& addr, uint16_t port);
    
    ~InetAddress();

    const sockaddr_in* getSockAddr() const {
        return &sockaddr;
    }

    socklen_t getSockLen() const {
        return sockaddr_len;
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