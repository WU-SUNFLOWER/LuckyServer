#include "connection.h"
#include "event_loop.h"
#include "server.h"
#include "socket.h"
#include "buffer.h"
#include "channel.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_socket)
    : loop(_loop), socket(_socket), channel(nullptr), inBuffer(nullptr), readBuffer(nullptr)
{
    channel = new Channel(loop, socket->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, socket->getFd());
    channel->setCallback(cb);
    channel->enableReading();

    inBuffer = new Buffer();
    readBuffer = new Buffer();
}

Connection::~Connection()
{
    delete channel;
    delete socket;
    delete inBuffer;
    delete readBuffer;
}

void Connection::echo(int client_socket_fd)
{
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(client_socket_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            readBuffer->append(buf, read_bytes);
        }
        else if (read_bytes == 0)
        {
            printf("client fd %d disconnected\n", client_socket_fd);
            deleteConnectionCallback(socket);
            break;
        }
        else if (read_bytes == -1 && errno == EINTR)
        {
            printf("continue reading...\n");
            continue;
        }
        else if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            printf("finish reading once, errno: %d\n", errno);
            printf("read %ld bytes mssage from client fd %d: %s\n",
                   readBuffer->size(),
                   client_socket_fd,
                   readBuffer->c_str());
            Write(client_socket_fd, readBuffer->c_str(), readBuffer->size());
            readBuffer->clear();
            break;
        }
        else
        {
            printErrorAndExit("unknown return value of read function.");
            deleteConnectionCallback(socket);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb)
{
    deleteConnectionCallback = _cb;
}
