#include "Connection.h"
#include "EventLoop.h"
#include "Server.h"
#include "Socket.h"
#include "Buffer.h"
#include "Channel.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_socket) 
    : loop(_loop), socket(_socket), channel(nullptr), inBuffer(nullptr), readBuffer(nullptr)
{
    channel = new Channel(loop, socket->getFd());
    channel->enableReading();
    channel->useET();

    std::function<void()> cb = std::bind(&Connection::echo, this);
    channel->setReadCallback(cb);

    inBuffer = new Buffer();
    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete channel;
    delete socket;
    delete inBuffer;
    delete readBuffer;
}

void Connection::echo() {
    char buf[READ_BUFFER];
    int client_socket_fd = socket->getFd();
    while (true) {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(client_socket_fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            readBuffer->append(buf, read_bytes);
        }
        else if (read_bytes == 0) {
            printf("client fd %d disconnected\n", client_socket_fd);
            deleteConnectionCallback(socket);
            break;
        } 
        else if (read_bytes == -1 && errno == EINTR) {
            printf("continue reading...\n");
            continue;
        }
        else if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("read %ld bytes mssage from client fd %d: %s\n", 
                readBuffer->size(), 
                client_socket_fd, 
                readBuffer->c_str()
            );
            send();
            readBuffer->clear();
            break;
        } 
        else {
            printErrorAndExit("unknown return value of read function.");
            deleteConnectionCallback(socket);
            break;
        }
    }
}

void Connection::send() {
    const char* buf = readBuffer->c_str();
    size_t data_size = readBuffer->size();
    size_t data_left = data_size;
    while (data_left > 0) {
        ssize_t write_bytes = write(socket->getFd(), buf + data_size - data_left, data_left);
        if (write_bytes == -1 && errno == EAGAIN) {
            break;
        }
        data_left -= write_bytes;
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb) {
    deleteConnectionCallback = _cb;
}
