#include "syscall.h"
#include "util.h"

int Socket(int __domain, int __type, int __protocol) {
    int ret = socket(__domain, __type, __protocol);
    errif(ret == -1, "socket create error");
    return ret;
}

int Bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len) {
    int ret = bind(__fd, __addr, __len);
    errif(ret == -1, "socket bind error");
    return ret;
}

int Listen(int __fd, int __n) {
    int ret = listen(__fd, __n);
    errif(ret == -1, "socket listen error");
    return ret;
}

int Accept(int __fd, __SOCKADDR_ARG __addr, socklen_t* __addr_len) {
    int ret = accept(__fd, __addr, __addr_len);
    errif(ret == -1, "socket accept error");
    return ret;
}

int Connect(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len) {
    int ret = connect(__fd, __addr, __len);
    errif(ret == -1, "socket connect error");
    return ret;
}

int EpollCreate1(int flag) {
    int ret = epoll_create1(flag);
    errif(ret == -1, "epoll create error");
    return ret;
}

int EpollCtl(int __epfd, int __op, int __fd, struct epoll_event* event) {
    int ret = epoll_ctl(__epfd, __op, __fd, event);
    errif(ret == -1, "epoll control error");
    return ret;
}

int EpollWait(int __epfd, struct epoll_event *__events, int __maxevents, int __timeout) {
    int ret = epoll_wait(__epfd, __events, __maxevents, __timeout);
    errif(ret == -1, "epoll wait error");
    return ret;
}