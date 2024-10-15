#include "Server.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "Channel.h"

Server::Server(EventLoop *_loop)
    : loop(_loop), acceptor(nullptr)
{
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(
        &Server::newConnection, 
        this, 
        std::placeholders::_1
    );
    acceptor->setNewConnectionCallBack(cb);
}

Server::~Server() {
    delete acceptor;
}

void Server::newConnection(Socket* clientSocket) {
    Connection* conn = new Connection(loop, clientSocket);
    std::function<void(Socket*)> cb = std::bind(
        &Server::deleteConnection, 
        this,
        std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[clientSocket->getFd()] = conn;
}

void Server::deleteConnection(Socket* socket) {
    Connection* conn = connections[socket->getFd()];
    connections.erase(socket->getFd());
    delete conn;
}
