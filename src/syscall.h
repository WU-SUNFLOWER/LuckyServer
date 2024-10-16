#ifndef LUCKYSERVER_SYSCALL_H_
#define LUCKYSERVER_SYSCALL_H_

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <cerrno>

namespace mysyscall
{
    extern int CreateSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    extern int Bind(int fd, __CONST_SOCKADDR_ARG address, socklen_t len);
    extern int Accept(int fd, __SOCKADDR_ARG address, socklen_t *addr_len);
    extern int Connect(int fd, __CONST_SOCKADDR_ARG address, socklen_t len);
    extern int Listen(int fd, int n);

    extern int EpollCreate1(int flag);
    extern int EpollCtl(int epfd, int op, int fd, struct epoll_event *event);
    extern int EpollWait(int epfd, struct epoll_event *events, int maxevents, int timeout);
    extern int Close(int fd);

    extern ssize_t Write(int fd, const void *kBuf, size_t n);
} // namespace mysyscall
#endif // LUCKYSERVER_SYSCALL_H_