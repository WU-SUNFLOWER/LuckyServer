#include "connection.h"

#include "event_loop.h"
#include "server.h"
#include "socket.h"
#include "buffer.h"
#include "channel.h"
#include "util.h"

#define TEMP_BUFFER 1024

Connection::Connection(EventLoop *loop, Socket *socket)
    : loop_(loop), socket_(socket), channel_(nullptr), send_buffer_(nullptr), read_buffer_(nullptr)
{
    util::ErrIf(loop == nullptr, "loop is nullptr");

    channel_ = new Channel(loop_, socket_->GetFd());
    channel_->EnableReading();
    channel_->UseET();

    send_buffer_ = new Buffer();
    read_buffer_ = new Buffer();

    state_ = State::Connected;
}

Connection::~Connection()
{
    delete channel_;
    delete socket_;
    delete send_buffer_;
    delete read_buffer_;
}

void Connection::Close()
{
    delete_connection_callback_(socket_);
}

void Connection::SetDeleteConnectionCallback(std::function<void(Socket *)> const &callback)
{
    delete_connection_callback_ = callback;
}

void Connection::SetOnConnectCallback(std::function<void(Connection *)> const &callback)
{
    on_connect_callback_ = callback;
    channel_->SetReadCallback([this]() {
        on_connect_callback_(this);
    });
}

Socket *Connection::GetSocket()
{
    return socket_;
}

Connection::State Connection::GetState() const
{
    return state_;
}

void Connection::Read()
{
    util::ErrIf(state_ != State::Connected, "connction state is disconnectied!");

    read_buffer_->Clear();
    if (socket_->IsNonBlocking()) {
        ReadNonBlocking();
    } else {
        ReadBlocking();
    }
}

void Connection::Write() {
    util::ErrIf(state_ != State::Connected, "connction state is disconnectied!");

    if (socket_->IsNonBlocking()) {
        WriteNonBlocking();
    } else {
        WriteBlocking();
    }
    send_buffer_->Clear();
}

void Connection::ReadBlocking()
{
    int socket_fd = socket_->GetFd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    ::getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);

    char buf[rcv_size];
    ssize_t read_bytes = ::read(socket_fd, buf, rcv_size);
    if (read_bytes > 0) {
        read_buffer_->Append(buf, read_bytes);
    }
    else if (read_bytes == 0) {
        printf("read EOF, blocking client fd %d disconnected\n", socket_fd);
        state_ = State::Closed;
    }
    else {
        printf("error on read blocking client fd %d\n", socket_fd);
        state_ = State::Closed;
    }
}

void Connection::WriteBlocking()
{
    int socket_fd = socket_->GetFd();
    ssize_t write_bytes = ::write(socket_fd, send_buffer_->ToStr(), send_buffer_->Size());

    if (write_bytes < 0) {
        printf("error on write blocking client fd %d\n", socket_fd);
        state_ = State::Closed;
    }
}

void Connection::ReadNonBlocking() 
{
    int socket_fd = socket_->GetFd();
    char buf[TEMP_BUFFER];

    while (true) {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = ::read(socket_fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            read_buffer_->Append(buf, read_bytes);
        }
        // luckyserver is interrupted
        // we just need to continue reading!
        else if (read_bytes == -1 && errno == EINTR) {
            continue;
        }
        // reading is finished
        else if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            break;
        }
        // we find EOF
        else if (read_bytes == 0) {
            printf("read EOF, client fd %d disconnected\n", socket_fd);
            state_ = State::Closed;
            break;
        }
        else {
            printf("error on client fd %d\n", socket_fd);
            state_ = State::Closed;
            break;
        }
    }
}

void Connection::WriteNonBlocking() 
{
    int socket_fd = socket_->GetFd();
    const char *buf = send_buffer_->ToStr();
    size_t data_size = send_buffer_->Size();
    size_t data_left = data_size;
    while (data_left > 0)
    {
        ssize_t write_bytes = write(socket_fd, buf + data_size - data_left, data_left);
        // continue writing...
        if (write_bytes == -1 && errno == EINTR) {
            continue;
        }
        // writing is finished
        if (write_bytes == -1 && errno == EAGAIN) {
            break;
        }
        // error
        if (write_bytes == -1) {
            printf("error on client fd %d\n", socket_fd);
            state_ = State::Closed;
            break;
        }
        data_left -= write_bytes;
    }
}

void Connection::SetSendBuffer(const char *str)
{
    send_buffer_->SetBuf(str);
}

const char *Connection::ReadBuffer()
{
    return read_buffer_->ToStr();
}

const std::string &Connection::ReadRawBuffer() {
    return read_buffer_->ToRaw();
}