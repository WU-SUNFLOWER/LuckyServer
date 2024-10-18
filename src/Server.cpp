#include "server.h"

#include <thread>
#include <functional>

#include "event_loop.h"
#include "socket.h"
#include "acceptor.h"
#include "connection.h"
#include "channel.h"
#include "thread_pool.h"
#include "util.h"

Server::Server(int port, EventLoop *loop)
    : port_(port), main_reactor_(loop), acceptor_(nullptr), thread_pool_(nullptr)
{
    acceptor_ = new Acceptor(port_, main_reactor_);
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
    util::ErrIf(client_socket->GetFd() == -1, "new connection error");

    int random = client_socket->GetFd() % sub_reactors_.size();
    Connection *connection = new Connection(sub_reactors_[random], client_socket);
    util::ErrIf(connection == nullptr, "new connection is nullptr");

    std::function<void(Connection *)> cb = std::bind(
        &Server::DeleteConnection,
        this,
        std::placeholders::_1);

    connection->SetOnConnectCallback(on_connect_callback_);
    connection->SetDeleteConnectionCallback(cb);
}

void Server::DeleteConnection(Connection *conn)
{
    delete conn;
}

void Server::OnConnect(std::function<void(Connection *)> const &fn)
{
    on_connect_callback_ = fn;
}