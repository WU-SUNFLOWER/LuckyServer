#include "http_server.h"

#include <functional>
#include <sstream>
#include <unordered_map>

#include "event_loop.h"
#include "server.h"
#include "connection.h"
#include "util.h"
#include "http_connection.h"

HttpServer::HttpServer()
    : loop_(nullptr), server_(nullptr)
{
    loop_ = new EventLoop();
    server_ = new Server(loop_);

    std::function<void(Connection *)> on_connect_callback = 
        std::bind(&HttpServer::OnConnect, this, std::placeholders::_1);
    server_->OnConnect(on_connect_callback);
}

HttpServer::~HttpServer()
{
    delete server_;
    delete loop_;
}

void HttpServer::Run()
{
    loop_->Loop();
}

void HttpServer::OnConnect(Connection *conn)
{
    HttpConnection http_conn(conn);
    if (!http_conn.ReadRequestMessage()) {
        return;
    }
    if (!http_conn.ParseRequestMessage()) {
        printf("fail to parse http request message, "
               "so we close this connection directly!\n");
        http_conn.Close();
        return;
    }
    //http_conn.PrintRequestMessage();
    http_conn.RespondSimply("", "200", "Successful", "Web Server is working!");

    if (http_conn.HasHeader("connection") 
        && http_conn.GetHeader("connection") == "close")
    {
        http_conn.Close();
        return;
    }
    
}
