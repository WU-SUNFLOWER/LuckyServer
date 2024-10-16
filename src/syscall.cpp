#include "syscall.h"
#include "util.h"

namespace mysyscall
{
    ssize_t Write(int fd, const void *kBuf, size_t n)
    {
        ssize_t ret = write(fd, kBuf, n);
        util::ErrIf(ret == -1, "write error");
        return ret;
    }

    int CreateSocket(int domain, int type, int protocol)
    {
        int ret = socket(domain, type, protocol);
        util::ErrIf(ret == -1, "socket create error");
        return ret;
    }

    int Bind(int fd, __CONST_SOCKADDR_ARG address, socklen_t len)
    {
        int ret = bind(fd, address, len);
        util::ErrIf(ret == -1, "socket bind error");
        return ret;
    }

    int Listen(int fd, int n)
    {
        int ret = listen(fd, n);
        util::ErrIf(ret == -1, "socket listen error");
        return ret;
    }

    int Accept(int fd, __SOCKADDR_ARG address, socklen_t *addr_len)
    {
        int ret = accept(fd, address, addr_len);
        util::ErrIf(ret == -1, "socket accept error");
        return ret;
    }

    int Connect(int fd, __CONST_SOCKADDR_ARG address, socklen_t len)
    {
        int ret = connect(fd, address, len);
        util::ErrIf(ret == -1, "socket connect error");
        return ret;
    }

    int EpollCreate1(int flag)
    {
        int ret = epoll_create1(flag);
        util::ErrIf(ret == -1, "epoll create error");
        return ret;
    }

    int EpollCtl(int epfd, int op, int fd, struct epoll_event *event)
    {
        int ret = epoll_ctl(epfd, op, fd, event);
        util::ErrIf(ret == -1, "epoll control error");
        return ret;
    }

    int EpollWait(int epfd, struct epoll_event *events, int maxevents, int timeout)
    {
        int ret = epoll_wait(epfd, events, maxevents, timeout);
        // reference: https://blog.csdn.net/Ilozk/article/details/90242543
        util::ErrIf(ret == -1 && errno != EINTR, "epoll wait error");
        return ret;
    }

    int Close(int fd)
    {
        int ret = close(fd);
        util::ErrIf(ret == -1, "close fd error");
        return ret;
    }
}