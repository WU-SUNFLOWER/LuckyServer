#include "socket.h"
#include "syscall.h"
#include "util.h"

Socket::Socket()
{
    socket_fd_ = CreateSocket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int fd) : socket_fd_(fd)
{
    errif(fd == -1, "illegal fd");
}

Socket::~Socket()
{
    this->close();
}

void Socket::close()
{
    if (socket_fd_ != -1)
    {
        Close(socket_fd_);
        socket_fd_ = -1;
    }
}

void Socket::bind(const InetAddress &kAddr)
{
    Bind(socket_fd_, (sockaddr *)kAddr.getSockAddr(), kAddr.getSockLen());
}

void Socket::listen()
{
    Listen(socket_fd_, SOMAXCONN);
}

int Socket::accept(InetAddress &clientAddr)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    int client_fd = Accept(socket_fd_, (sockaddr *)&client_addr, &client_addr_len);
    clientAddr.setInetAddr(client_addr);

    return client_fd;
}

void Socket::setNonBlocking()
{
    fcntl(socket_fd_, F_SETFL, fcntl(socket_fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::getFd()
{
    return socket_fd_;
}