#include "socket.h"
#include "syscall.h"
#include "util.h"

Socket::Socket()
{
    socket_fd_ = mysyscall::CreateSocket(AF_INET, SOCK_STREAM, 0);
    util::ErrIf(socket_fd_ == -1, "CreateSocket error");
}

Socket::Socket(int fd) : socket_fd_(fd)
{
    util::ErrIf(fd == -1, "illegal fd");
}

Socket::~Socket()
{
    Close();
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
    util::ErrIf(mysyscall::Bind(socket_fd_, (sockaddr *)kAddr.GetSockAddress(), kAddr.GetSockLen()) == -1,
                "Bind error");
}

void Socket::Listen()
{
    util::ErrIf(mysyscall::Listen(socket_fd_, SOMAXCONN) == -1, "Listen error");
}

int Socket::Accept(InetAddress &clientAddr)
{
    struct sockaddr_in client_addr
    {
    };
    socklen_t client_addr_len = sizeof(client_addr);

    int client_fd = mysyscall::Accept(socket_fd_, (sockaddr *)&client_addr, &client_addr_len);
    util::ErrIf(client_fd == -1, "Accept error");
    clientAddr.SetInetAddress(client_addr);

    return client_fd;
}

void Socket::Connect(const InetAddress &addr)
{
    const sockaddr_in *address = addr.GetSockAddress();
    if (fcntl(socket_fd_, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            int ret = mysyscall::Connect(socket_fd_, (sockaddr *)address, sizeof(sockaddr_in));
            if (ret == 0)
            {
                break;
            }
            else if (ret == -1 && errno == EINPROGRESS)
            {
                continue;
            }
            else if (ret == -1)
            {
                util::PrintErrorAndExit("socket connect error");
            }
        }
    }
    else
    {
        mysyscall::Connect(socket_fd_, (sockaddr *)address, sizeof(sockaddr_in));
    }
}

void Socket::SetNonBlocking()
{
    int flags = fcntl(socket_fd_, F_GETFL, 0);
    util::ErrIf(flags == -1, "fcntl F_GETFL error");
    util::ErrIf(fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK) == -1, "fcntl F_SETFL error");
}

int Socket::GetFd() const
{
    return socket_fd_;
}