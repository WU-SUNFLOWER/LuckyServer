#include "inet_address.h"

InetAddress::InetAddress()
{
    sockaddr_len_ = sizeof(sockaddr_);
    bzero(&sockaddr_, sockaddr_len_);
}

InetAddress::InetAddress(const std::string &kAddr, uint16_t port)
    : InetAddress()
{
    sockaddr_.sin_family = AF_INET;
    sockaddr_.sin_addr.s_addr = inet_addr(kAddr.c_str());
    sockaddr_.sin_port = htons(port);
}

InetAddress::~InetAddress()
{
}

void InetAddress::SetInetAddress(struct sockaddr_in &newSockAddr)
{
    sockaddr_.sin_family = newSockAddr.sin_family;
    sockaddr_.sin_addr.s_addr = newSockAddr.sin_addr.s_addr;
    sockaddr_.sin_port = newSockAddr.sin_port;
}