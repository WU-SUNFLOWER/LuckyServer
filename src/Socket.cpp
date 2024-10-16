#include "socket.h"
#include "syscall.h"
#include "util.h"

Socket::Socket()
{
    socket_fd_ = mysyscall::CreateSocket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int fd) : socket_fd_(fd)
{
    util::ErrIf(fd == -1, "illegal fd");
}

Socket::~Socket()
{
    this->Close();
}

void Socket::Close()
{
    if (socket_fd_ != -1)
    {
        mysyscall::Close(socket_fd_);
        socket_fd_ = -1;
    }
}

void Socket::Bind(const InetAddress &kAddr)
{
    mysyscall::Bind(socket_fd_, (sockaddr *)kAddr.GetSockAddress(), kAddr.GetSockLen());
}

void Socket::Listen()
{
    mysyscall::Listen(socket_fd_, SOMAXCONN);
}

int Socket::Accept(InetAddress &clientAddr)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    int client_fd = mysyscall::Accept(socket_fd_, (sockaddr *)&client_addr, &client_addr_len);
    clientAddr.SetInetAddress(client_addr);

    return client_fd;
}

void Socket::SetNonBlocking()
{
    fcntl(socket_fd_, F_SETFL, fcntl(socket_fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::GetFd()
{
    return socket_fd_;
}