#include "acceptor.h"

Acceptor::Acceptor(EventLoop *loop)
    : loop_(loop)
{
    socket_ = new Socket();
    address_ = {"127.0.0.1", 8888};

    socket_->Bind(address_);
    socket_->Listen();
    socket_->SetNonBlocking();

    accept_channel_ = new Channel(loop_, socket_->GetFd());
    std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);

    accept_channel_->SetCallback(cb);
    accept_channel_->EnableReading();
}

Acceptor::~Acceptor()
{
    delete socket_;
    delete accept_channel_;
}

void Acceptor::AcceptConnection()
{
    InetAddress client_address;
    Socket *client_socket = new Socket(socket_->Accept(client_address));
    client_socket->SetNonBlocking();

    printf("New client fd %d! From IP: %s Port: %d\n",
           client_socket->GetFd(),
           client_address.GetIP().c_str(),
           client_address.GetPort());

    NewConnectionCallback(client_socket);
}

void Acceptor::SetNewConnectionCallBack(std::function<void(Socket *)> cb)
{
    NewConnectionCallback = cb;
}
