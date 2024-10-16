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

int CreateSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
int Bind(int fd, __CONST_SOCKADDR_ARG address, socklen_t len);
int Accept(int fd, __SOCKADDR_ARG address, socklen_t *addr_len);
int Connect(int fd, __CONST_SOCKADDR_ARG address, socklen_t len);
int Listen(int fd, int n);

int EpollCreate1(int flag);
int EpollCtl(int epfd, int op, int fd, struct epoll_event *event);
int EpollWait(int epfd, struct epoll_event *events, int maxevents, int timeout);
int Close(int fd);

ssize_t Write(int fd, const void *kBuf, size_t n);

#endif