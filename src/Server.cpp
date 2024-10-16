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
        &Server::NewConnection,
        this,
        std::placeholders::_1);
    acceptor_->SetNewConnectionCallBack(cb);
}

Server::~Server()
{
    delete acceptor_;
}

void Server::NewConnection(Socket *clientSocket)
{
    Connection *connection = new Connection(loop_, clientSocket);
    std::function<void(Socket *)> cb = std::bind(
        &Server::DeleteConnection,
        this,
        std::placeholders::_1);
    connection->SetDeleteConnectionCallback(cb);
    connections_[clientSocket->GetFd()] = connection;
}

void Server::DeleteConnection(Socket *socket)
{
    Connection *connection = connections_[socket->GetFd()];
    connections_.erase(socket->GetFd());
    delete connection;
}
