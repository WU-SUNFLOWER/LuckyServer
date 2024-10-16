#include "server.h"

#include <thread>
#include <functional>

#include "event_loop.h"
#include "socket.h"
#include "acceptor.h"
#include "connection.h"
#include "channel.h"
#include "thread_pool.h"

Server::Server(EventLoop *loop)
    : main_reactor_(loop), acceptor_(nullptr)
{
    acceptor_ = new Acceptor(main_reactor_);
    std::function<void(Socket *)> cb = std::bind(
        &Server::NewConnection,
        this,
        std::placeholders::_1);
    acceptor_->SetNewConnectionCallBack(cb);

    int reactor_num = std::thread::hardware_concurrency();
    thread_pool_ = new ThreadPool(reactor_num);
    for (int i = 0; i < reactor_num; ++i)
    {
        EventLoop *sub_reactor = new EventLoop();
        sub_reactors_.push_back(sub_reactor);
        std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i]);
        thread_pool_->AddTask(sub_loop);
    }
}

Server::~Server()
{
    delete acceptor_;
    delete thread_pool_;
}

void Server::NewConnection(Socket *client_socket)
{
    if (client_socket->GetFd() != -1)
    {
        int random = client_socket->GetFd() % sub_reactors_.size();
        Connection *connection = new Connection(sub_reactors_[random], client_socket);

        std::function<void(Socket *)> cb = std::bind(
            &Server::DeleteConnection,
            this,
            std::placeholders::_1);
        connection->SetDeleteConnectionCallback(cb);
        connections_[client_socket->GetFd()] = connection;
    }
}

void Server::DeleteConnection(Socket *socket)
{
    Connection *connection = connections_[socket->GetFd()];
    connections_.erase(socket->GetFd());
    delete connection;
}
