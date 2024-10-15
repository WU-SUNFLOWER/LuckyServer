#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *_loop)
    : loop(_loop)
{
    socket = new Socket();
    addr = {"127.0.0.1", 8888};
    
    socket->bind(addr);
    socket->listen();
    socket->setNonBlocking();

    acceptChannel = new Channel(loop, socket->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);

    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor() {
    delete socket;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    newConnectionCallback(socket);
}

void Acceptor::setNewConnectionCallBack(std::function<void(Socket *)> _cb) {
    newConnectionCallback = _cb;
}
