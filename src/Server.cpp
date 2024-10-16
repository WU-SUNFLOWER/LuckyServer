#include "server.h"
#include "event_loop.h"
#include "socket.h"
#include "acceptor.h"
#include "connection.h"
#include "channel.h"

Server::Server(EventLoop *_loop)
    : loop(_loop), acceptor(nullptr)
{
    acceptor = new Acceptor(loop);
    std::function<void(Socket *)> cb = std::bind(
        &Server::newConnection,
        this,
        std::placeholders::_1);
    acceptor->setNewConnectionCallBack(cb);
}

Server::~Server()
{
    delete acceptor;
}

void Server::newConnection(Socket *clientSocket)
{
    Connection *conn = new Connection(loop, clientSocket);
    std::function<void(Socket *)> cb = std::bind(
        &Server::deleteConnection,
        this,
        std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[clientSocket->getFd()] = conn;
}

void Server::deleteConnection(Socket *socket)
{
    Connection *conn = connections[socket->getFd()];
    connections.erase(socket->getFd());
    delete conn;
}
