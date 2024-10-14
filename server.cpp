#include "src/Socket.h"
#include "src/InetAddress.h"
#include "src/Epoll.h"
#include "src/Channel.h"
#include <errno.h>

#define READ_BUFFER 1024

void handleReadEvent(int client_socket_fd) {
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

int main() {

    Socket servSocket;
    InetAddress servAddr("127.0.0.1", 8888);
    servSocket.bind(servAddr);
    servSocket.listen();

    Epoll servEpoll;
    servSocket.setNonBlocking();

    Channel servChannel(&servEpoll, servSocket.getFd());
    servChannel.enableReading();

    while (true) {
        std::vector<Channel*> channels = servEpoll.wait();
        for (Channel* channel: channels) {
            if (channel->getFd() == servSocket.getFd()) {
                InetAddress clientAddr;
                Socket* clientSocket = new Socket(servSocket.accept(clientAddr));

                printf("New client fd %d! From IP: %s Port: %d\n", 
                        clientSocket->getFd(), 
                        clientAddr.getIP().c_str(),
                        clientAddr.getPort()
                );

                clientSocket->setNonBlocking();

                Channel* clientChannel = new Channel(&servEpoll, clientSocket->getFd());
                clientChannel->enableReading();
            }
            else if (channel->getRevents() & EPOLLIN) {
                handleReadEvent(channel->getFd());
            }
            else {
                printf("something else happened\n");
            }
        }
    }
    
    return 0;
}