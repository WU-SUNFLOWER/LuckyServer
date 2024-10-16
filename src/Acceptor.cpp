#include "acceptor.h"

Acceptor::Acceptor(EventLoop *loop)
    : loop_(loop)
{
    socket_ = new Socket();
    address_ = {"127.0.0.1", 8888};

    socket_->bind(address_);
    socket_->listen();
    socket_->setNonBlocking();

    accept_channel_ = new Channel(loop_, socket_->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);

    accept_channel_->setCallback(cb);
    accept_channel_->enableReading();
}

Acceptor::~Acceptor()
{
    delete socket_;
    delete accept_channel_;
}

void Acceptor::acceptConnection()
{
    InetAddress client_address;
    Socket *client_socket = new Socket(socket_->accept(client_address));
    client_socket->setNonBlocking();

    printf("New client fd %d! From IP: %s Port: %d\n",
           client_socket->getFd(),
           client_address.getIP().c_str(),
           client_address.getPort());

    newConnectionCallback(client_socket);
}

void Acceptor::setNewConnectionCallBack(std::function<void(Socket *)> cb)
{
    newConnectionCallback = cb;
}
