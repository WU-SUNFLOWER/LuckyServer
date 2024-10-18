#include "http_server.h"

#include <functional>
#include <sstream>
#include <unordered_map>
#include <sys/stat.h>

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
    // read request header and body
    if (!http_conn.ReadRequestMessage()) {
        http_conn.Close();
        return;
    }
    // parse request header
    if (!http_conn.ParseRequestMessage()) {
        printf("fail to parse http request message, "
               "so we close this connection directly!\n");
        http_conn.Close();
        return;
    }
    // now we can only deal with GET request!
    if (http_conn.GetMethod() != "get") {
        http_conn.RespondSimply(
            http_conn.GetMethod().c_str(), 
            "501", 
            "Not Implemented",
            "Lucky Web Server does not implement this method"
        );
        http_conn.Close();
        return;
    }

    std::string file_path = http_conn.ParseURI();
    struct stat sbuf {};
    // does the file requested exist?
    if (::stat(file_path.c_str(), &sbuf) < 0) {
        http_conn.RespondSimply(
            file_path.c_str(), 
            "404", 
            "Not found",
		    "Lucky Web Server couldn't find this file"
        );
	    http_conn.Close();
        return;
    }

    // do we have the permission to access the file?
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
        http_conn.RespondSimply(
            file_path.c_str(), 
            "403", 
            "Forbidden",
		    "Lucky Web Server couldn't read the file"
        );
	    http_conn.Close();
        return;
	}

    http_conn.RespondStaticFile(file_path, sbuf.st_size);

    if (http_conn.HasHeader("connection") 
        && http_conn.GetHeader("connection") == "close")
    {
        http_conn.Close();
        return;
    }
    
}
