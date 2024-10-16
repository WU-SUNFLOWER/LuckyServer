#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *_loop)
    : loop(_loop)
{
    socket = new Socket();
    addr = {"127.0.0.1", 8888};
    
    socket->bind(addr);
    socket->listen();

    acceptChannel = new Channel(loop, socket->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);

    acceptChannel->setReadCallback(cb);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor() {
    delete socket;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    InetAddress clientAddr;

    printf("Acceptor::acceptConnection\n");

    Socket* clientSocket = new Socket(socket->accept(clientAddr));
    clientSocket->setNonBlocking();

    printf("New client fd %d! From IP: %s Port: %d\n", 
            clientSocket->getFd(), 
            clientAddr.getIP().c_str(),
            clientAddr.getPort()
    );

    newConnectionCallback(clientSocket);
}

void Acceptor::setNewConnectionCallBack(std::function<void(Socket *)> _cb) {
    newConnectionCallback = _cb;
}
