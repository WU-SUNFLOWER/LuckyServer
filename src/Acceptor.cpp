#include "acceptor.h"
#include "socket.h"
#include "channel.h"
#include "server.h"

Acceptor::Acceptor(int port, EventLoop *loop)
    : port_(port),
      loop_(loop),
      socket_(new Socket()),
      address_("0.0.0.0", port_),
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
    Socket *client_socket = new Socket(socket_->Accept(client_address));
    client_socket->SetNonBlocking();

    util::DebugPrint("New client fd %d! From IP: %s Port: %d\n",
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