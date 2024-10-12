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

int Socket(int __domain, int __type, int __protocol);
int Bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
int Accept(int __fd, __SOCKADDR_ARG __addr, socklen_t* __addr_len);
int Connect(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
int Listen(int __fd, int __n);

int EpollCreate1(int flag);
int EpollCtl(int __epfd, int __op, int __fd, struct epoll_event* event);
int EpollWait(int __epfd, struct epoll_event *__events, int __maxevents, int __timeout);

#endif