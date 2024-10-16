#include "inet_address.h"

InetAddress::InetAddress()
{
    sockaddr_len_ = sizeof(sockaddr);
    bzero(&sockaddr, sockaddr_len_);
}

InetAddress::InetAddress(const std::string &kAddr, uint16_t port)
    : InetAddress()
{
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(kAddr.c_str());
    sockaddr.sin_port = htons(port);
}

InetAddress::~InetAddress()
{
}

void InetAddress::setInetAddr(struct sockaddr_in &newSockAddr)
{
    sockaddr.sin_family = newSockAddr.sin_family;
    sockaddr.sin_addr.s_addr = newSockAddr.sin_addr.s_addr;
    sockaddr.sin_port = newSockAddr.sin_port;
}