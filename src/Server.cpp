#include "Server.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "Channel.h"
#include "ThreadPool.h"
#include <thread>
#include <functional>

Server::Server(EventLoop *_loop)
    : mainReactor(_loop), acceptor(nullptr)
{
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(
        &Server::newConnection, 
        this, 
        std::placeholders::_1
    );
    acceptor->setNewConnectionCallBack(cb);

    int reactorNum = std::thread::hardware_concurrency();
    threadPool = new ThreadPool(reactorNum);
    for (int i = 0; i < reactorNum; ++i) {
        EventLoop* subReactor = new EventLoop();
        subReactors.push_back(subReactor);

        std::function<void()> subLoop = std::bind(&EventLoop::loop, subReactors[i]);
        threadPool->addTask(subLoop);
    }
}

Server::~Server() {
    delete acceptor;
    delete threadPool;
}

void Server::newConnection(Socket* clientSocket) {
    if (clientSocket->getFd() != -1) {
        int random = clientSocket->getFd() % subReactors.size();
        Connection* conn = new Connection(subReactors[random], clientSocket);
        
        std::function<void(Socket*)> cb = std::bind(
            &Server::deleteConnection, 
            this,
            std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[clientSocket->getFd()] = conn;
    }
}

void Server::deleteConnection(Socket* socket) {
    Connection* conn = connections[socket->getFd()];
    connections.erase(socket->getFd());
    delete conn;
}
