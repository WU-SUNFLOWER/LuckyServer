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
#include "syscall.h"

HttpServer::HttpServer(int port, const std::filesystem::path &work_dir)
    : loop_(nullptr), server_(nullptr), work_dir_(work_dir)
{
    loop_ = new EventLoop();
    server_ = new Server(port, loop_);

    if (!std::filesystem::exists(work_dir_) || !std::filesystem::is_directory(work_dir_)) {
        util::PrintErrorAndExit("illegal work directory!");
        return;
    }

    mysyscall::Chdir(work_dir_.c_str());

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
        util::DebugPrint("fail to parse http request message, "
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

    http_conn.Close();
    
}
