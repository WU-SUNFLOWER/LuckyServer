#include "acceptor.h"
#include "socket.h"
#include "channel.h"

Acceptor::Acceptor(EventLoop *loop)
    : loop_(loop),
      socket_(new Socket()),
      address_("127.0.0.1", 8888),
      accept_channel_(nullptr)
{
    socket_->Bind(address_);
    socket_->Listen();

    accept_channel_ = new Channel(loop_, socket_->GetFd());
    accept_channel_->SetReadCallback(std::bind(&Acceptor::AcceptConnection, this));
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
    printf("Acceptor::acceptConnection\n");
    Socket *client_socket = new Socket(socket_->Accept(client_address));
    client_socket->SetNonBlocking();

    printf("New client fd %d! From IP: %s Port: %d\n",
           client_socket->GetFd(),
           client_address.GetIP().c_str(),
           client_address.GetPort());

    if (new_connection_callback_)
    {
        new_connection_callback_(client_socket);
    }
}

void Acceptor::SetNewConnectionCallBack(std::function<void(Socket *)> const &callback)
{
    new_connection_callback_ = callback;
}
