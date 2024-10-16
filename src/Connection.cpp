#include "connection.h"

#include "event_loop.h"
#include "server.h"
#include "socket.h"
#include "buffer.h"
#include "channel.h"

const int kReadBuffer = 1024;

Connection::Connection(EventLoop *loop, Socket *socket)
    : loop_(loop), socket_(socket), channel_(nullptr), in_buffer_(nullptr), read_buffer_(nullptr)
{
    channel_ = new Channel(loop_, socket_->GetFd());
    std::function<void()> callback = std::bind(&Connection::Echo, this, socket_->GetFd());
    channel_->SetCallback(callback);
    channel_->EnableReading();

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

void Connection::Echo(int client_socket_fd)
{
    char buf[kReadBuffer];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(client_socket_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            read_buffer_->Append(buf, read_bytes);
        }
        else if (read_bytes == 0)
        {
            printf("client fd %d disconnected\n", client_socket_fd);
            delete_connection_callback_(socket_);
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
                   read_buffer_->Size(),
                   client_socket_fd,
                   read_buffer_->Cstr());
            mysyscall::Write(client_socket_fd, read_buffer_->Cstr(), read_buffer_->Size());
            read_buffer_->Clear();
            break;
        }
        else
        {
            util::PrintErrorAndExit("unknown return value of read function.");
            delete_connection_callback_(socket_);
            break;
        }
    }
}

void Connection::SetDeleteConnectionCallback(std::function<void(Socket *)> callback)
{
    delete_connection_callback_ = callback;
}
