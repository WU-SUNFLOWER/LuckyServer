#include "connection.h"
#include "event_loop.h"
#include "server.h"
#include "socket.h"
#include "buffer.h"
#include "channel.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *loop, Socket *socket)
    : loop_(loop), socket_(socket), channel_(nullptr), in_buffer_(nullptr), read_buffer_(nullptr)
{
    channel_ = new Channel(loop_, socket_->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, socket_->getFd());
    channel_->setCallback(cb);
    channel_->enableReading();

    in_buffer_ = new Buffer();
    read_buffer_ = new Buffer();
}

Connection::~Connection()
{
    delete channel_;
    delete socket_;
    delete in_buffer_;
    delete read_buffer_;
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
            read_buffer_->append(buf, read_bytes);
        }
        else if (read_bytes == 0)
        {
            printf("client fd %d disconnected\n", client_socket_fd);
            deleteConnectionCallback(socket_);
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
                   read_buffer_->size(),
                   client_socket_fd,
                   read_buffer_->c_str());
            Write(client_socket_fd, read_buffer_->c_str(), read_buffer_->size());
            read_buffer_->clear();
            break;
        }
        else
        {
            printErrorAndExit("unknown return value of read function.");
            deleteConnectionCallback(socket_);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb)
{
    deleteConnectionCallback = _cb;
}
