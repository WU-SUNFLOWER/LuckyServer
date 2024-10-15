#include "Server.h"

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop)
    : loop(_loop), acceptor(nullptr)
{
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(
        &Server::newConnection, 
        this, 
        std::placeholders::_1
    );
    acceptor->setNewConnectionCallBack(cb);
}

Server::~Server() {
    delete acceptor;
}

void Server::handleReadEvent(int client_socket_fd) {
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

void Server::newConnection(Socket* servSocket) {
    InetAddress clientAddr;
    Socket* clientSocket = new Socket(servSocket->accept(clientAddr));
    clientSocket->setNonBlocking();

    printf("New client fd %d! From IP: %s Port: %d\n", 
            clientSocket->getFd(), 
            clientAddr.getIP().c_str(),
            clientAddr.getPort()
    );

    Channel* clientChannel = new Channel(loop, clientSocket->getFd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clientSocket->getFd());
    clientChannel->setCallback(cb);
    clientChannel->enableReading();
}
