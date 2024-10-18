#include "http_connection.h"

#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "util.h"
#include "connection.h"

#define MAX_LINE 1024

const std::unordered_map<std::string, std::string> HttpConnection::MimeTypes = {
    {"html", "text/html"},
    {"css", "text/css"},
    {"js", "application/javascript"},
    {"gif", "image/gif"},
    {"png", "image/png"},
    {"jpeg", "image/jpeg"},
    {"jpg", "image/jpeg"},
};

std::string HttpConnection::GetFileType(const std::string &filename)
{
    std::string suffix = util::GetFilenameSuffix(filename);
    auto iter = MimeTypes.find(suffix);
    if (iter != MimeTypes.end()) {
        return iter->second;
    }
    return "application/octet-stream";
}

HttpConnection::HttpConnection(Connection *conn)
    : conn_(conn)
{
}

HttpConnection::~HttpConnection()
{
}

const std::string &HttpConnection::GetMethod()
{
    return method_;
}

const std::string &HttpConnection::GetURI()
{
    return uri_;
}

const std::string &HttpConnection::GetHttpVersion()
{
    return http_version_;
}

bool HttpConnection::HasHeader(const std::string &key)
{
    return headers_.find(key) != headers_.end();
}

const std::string &HttpConnection::GetHeader(const std::string &key)
{
    return headers_[key];
}

bool HttpConnection::ReadRequestMessage() {
    conn_->Read();
    if (conn_->GetState() == Connection::State::Closed) {
        return false;
    }
    return true;
}

bool HttpConnection::ParseRequestMessage()
{

    std::istringstream request_stream(conn_->ReadRawBuffer());

    // extract request line
    std::string request_line;
    std::getline(request_stream, request_line);
    if (request_line.empty()) {
        return false;
    }
    if (request_line.back() == '\r') {
        request_line.pop_back();
    }

    // parse request line
    util::ToLower(request_line);
    std::istringstream request_line_stream(request_line);
    request_line_stream >> method_ >> uri_ >> http_version_;

    // extract and parse request header
    std::string header_line;
    while (std::getline(request_stream, header_line) && header_line != "\r") {
        // safe check
        if (header_line.empty()) {
            return false;
        }
        if (header_line.back() == '\r') {
            header_line.pop_back();
        }
        // extract header key and value
        size_t colon_pos = header_line.find(':');
        if (colon_pos == std::string::npos) {
            return false;
        }
        std::string key = header_line.substr(0, colon_pos);
        std::string value = header_line.substr(colon_pos + 1);
        util::ToLower(util::Trim(key));
        util::ToLower(util::Trim(value));
        headers_[key] = value;
    }
    return true;
}

void HttpConnection::PrintRequestMessage()
{
    printf("Method: %s\nURI: %s\nVersion: %s\n", 
        method_.c_str(), uri_.c_str(), http_version_.c_str());
    printf("Headers:\n");
    for (const auto &header : headers_) {
        printf("%s: %s\n", header.first.c_str(), header.second.c_str());
    }
}

std::string HttpConnection::ParseURI()
{
    std::string result = '.' + uri_;
    if (result.back() == '/') {
        result += "index.html";
    }
    return result;
}


#define SetBufferAndSent(conn, buffer) \
    {                                  \
       conn->SetSendBuffer(buffer);    \
       conn->Write();                  \
    }

void HttpConnection::RespondSimply(const char *cause, const char *errnum, 
    const char *shortmsg, const char *longmsg)
{
    char line_buf[MAX_LINE];
    char respond_body_buf[MAX_LINE];

    sprintf(line_buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    SetBufferAndSent(conn_, line_buf);
    sprintf(line_buf, "Content-type: text/html\r\n");
    SetBufferAndSent(conn_, line_buf);

    sprintf(respond_body_buf, 
        "<html><title>Respond Simply</title>"
        "<body bgcolor=""ffffff"">\r\n"
        "%s: %s\r\n"
        "<p>%s: %s\r\n"
        "<hr><em>Lucky Web Server</em>\r\n"
        "</body></html>\r\n",
        errnum, shortmsg,
        longmsg, cause);

    sprintf(line_buf, "Content-Length: %ld\r\n\r\n", strlen(respond_body_buf));
    SetBufferAndSent(conn_, line_buf);

    SetBufferAndSent(conn_, respond_body_buf);

}

void HttpConnection::RespondStaticFile(const std::string &file_path, size_t file_size)
{
    char line_buf[MAX_LINE];
    std::string file_type = GetFileType(file_path);

    sprintf(line_buf, "HTTP/1.0 200 OK\r\n");
    SetBufferAndSent(conn_, line_buf);
    sprintf(line_buf, "Server: Lucky Web Server\r\n");
    SetBufferAndSent(conn_, line_buf);
    sprintf(line_buf, "Content-length: %ld\r\n", file_size);
    SetBufferAndSent(conn_, line_buf);
    sprintf(line_buf, "Content-type: %s\r\n\r\n", file_type.c_str());
    SetBufferAndSent(conn_, line_buf);

    int file_fd = ::open(file_path.c_str(), O_RDONLY, 0);
    char* file_src = (char*)::mmap(0, file_size, PROT_READ, MAP_PRIVATE, file_fd, 0);
    ::close(file_fd);
    conn_->SetSendBuffer(file_src, file_size); 
    conn_->Write();
    ::munmap(file_src, file_size);
}

#undef SetBufferAndSent

void HttpConnection::Close()
{
    conn_->Close();
}
