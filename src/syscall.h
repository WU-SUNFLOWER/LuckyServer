#ifndef HEAD_SYSCALL
#define HEAD_SYSCALL

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <cerrno>

int CreateSocket(int __domain = AF_INET, int __type = SOCK_STREAM, int __protocol = 0);
int Bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
int Accept(int __fd, __SOCKADDR_ARG __addr, socklen_t *__addr_len);
int Connect(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
int Listen(int __fd, int __n);

int EpollCreate1(int flag);
int EpollCtl(int __epfd, int __op, int __fd, struct epoll_event *event);
int EpollWait(int __epfd, struct epoll_event *__events, int __maxevents, int __timeout);
int Close(int fd);

ssize_t Write(int __fd, const void *__buf, size_t __n);

#endif