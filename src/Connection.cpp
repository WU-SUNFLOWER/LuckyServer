#include "Connection.h"
#include "EventLoop.h"
#include "Server.h"
#include "Socket.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_socket) 
    : loop(_loop), socket(_socket), channel(nullptr)
{
    channel = new Channel(loop, socket->getFd());
    
    std::function<void()> cb = std::bind(&Connection::echo, this, socket->getFd());
    channel->setCallback(cb);

    channel->enableReading();
}

Connection::~Connection() {
    delete channel;
    delete socket;
}

void Connection::echo(int client_socket_fd) {
    char buf[READ_BUFFER];
    while (true) {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(client_socket_fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            printf("read %ld bytes mssage from client fd %d: %s\n", read_bytes, client_socket_fd, buf);
            write(client_socket_fd, buf, sizeof(buf));
        } 
        else if (read_bytes == 0) {
            printf("client fd %d disconnected\n", client_socket_fd);
            close(client_socket_fd);
            break;
        } 
        else if (read_bytes == -1 && errno == EINTR) {
            printf("continue reading...\n");
            continue;
        } 
        else if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("finish reading once, errno: %d\n", errno);
            break;
        } 
        else {
            close(client_socket_fd);
            printErrorAndExit("unknown return value of read function.");
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb) {
    deleteConnectionCallback = _cb;
}
