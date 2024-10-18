#ifndef LUCKYSERVER_ACCEPTOR_H_
#define LUCKYSERVER_ACCEPTOR_H_

#include <filesystem>

class Server;
class EventLoop;
class Connection;

class HttpServer 
{
private:
    EventLoop *loop_;
    Server *server_;
    std::filesystem::path work_dir_;

public:

    explicit HttpServer(int port, const std::filesystem::path &work_dir);

    ~HttpServer();

    void Run();

    void OnConnect(Connection *);
};

#endif