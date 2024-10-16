#include "server.h"
#include "event_loop.h"
#include "socket.h"
#include "acceptor.h"
#include "connection.h"
#include "channel.h"

Server::Server(EventLoop *loop)
    : loop_(loop), acceptor_(nullptr)
{
    acceptor_ = new Acceptor(loop_);
    std::function<void(Socket *)> cb = std::bind(
        &Server::newConnection,
        this,
        std::placeholders::_1);
    acceptor_->setNewConnectionCallBack(cb);
}

Server::~Server()
{
    delete acceptor_;
}

void Server::newConnection(Socket *clientSocket)
{
    Connection *connection = new Connection(loop_, clientSocket);
    std::function<void(Socket *)> cb = std::bind(
        &Server::deleteConnection,
        this,
        std::placeholders::_1);
    connection->setDeleteConnectionCallback(cb);
    connections_[clientSocket->getFd()] = connection;
}

void Server::deleteConnection(Socket *socket)
{
    Connection *connection = connections_[socket->getFd()];
    connections_.erase(socket->getFd());
    delete connection;
}
