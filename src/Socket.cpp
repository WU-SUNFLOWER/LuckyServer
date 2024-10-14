#include "Socket.h"
#include "syscall.h"
#include "util.h"

Socket::Socket() {
    socket_fd = CreateSocket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int fd) : socket_fd(fd) {
    errif(fd == -1, "illegal fd");
}

Socket::~Socket() {
    this->close();
}

void Socket::close() {
    if (socket_fd != -1) {
        Close(socket_fd);
        socket_fd = -1;
    }
}

void Socket::bind(const InetAddress& addr) {
    Bind(socket_fd, (sockaddr*)addr.getSockAddr(), addr.getSockLen());
}

void Socket::listen() {
    Listen(socket_fd, SOMAXCONN);
}

int Socket::accept(InetAddress& clientAddr) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    int client_fd = Accept(socket_fd, (sockaddr*)&client_addr, &client_addr_len);
    clientAddr.setInetAddr(client_addr);
    
    return client_fd;
}

void Socket::setNonBlocking() {
    fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL) | O_NONBLOCK);  
}

int Socket::getFd() {
    return socket_fd;
}