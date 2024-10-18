#ifndef LUCKYSERVER_CONNECTION_H_
#define LUCKYSERVER_CONNECTION_H_

#include <functional>
#include <string>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection
{
public:
    enum State {
        Invalid = 1,
        Handshaking = 2,
        Connected = 3,
        Closed = 4,
        Failed = 5,
    };

private:
    EventLoop *loop_;
    Socket *socket_;
    Channel *channel_;

    std::function<void(Socket *)> delete_connection_callback_;
    std::function<void(Connection *)> on_connect_callback_;

    Buffer *send_buffer_;
    Buffer *read_buffer_;

    State state_;

public:
    explicit Connection(EventLoop *loop, Socket *socket);

    ~Connection();

    Socket *GetSocket();

    State GetState() const;

    void Read();
    void Write();

    void ReadBlocking();
    void WriteBlocking();

    void ReadNonBlocking();
    void WriteNonBlocking();

    void SetSendBuffer(const char *str);
    void SetSendBuffer(const char *str, size_t n);

    const char *ReadBuffer();
    const std::string &ReadRawBuffer();

    //void Send();
    void Close();

    void SetDeleteConnectionCallback(std::function<void(Socket *)> const &);

    void SetOnConnectCallback(std::function<void(Connection *)> const &);
};

#endif // LUCKYSERVER_CONNECTION_H_