#include "http_connection.h"

#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstring>

#include "util.h"
#include "connection.h"

#define MAX_LINE 1024

HttpConnection::HttpConnection(Connection *conn)
    : conn_(conn)
{
}

HttpConnection::~HttpConnection()
{
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

void HttpConnection::RespondSimply(const char *cause, const char *errnum, 
    const char *shortmsg, const char *longmsg)
{
    char line_buf[MAX_LINE];
    char respond_body_buf[MAX_LINE];

#define SetBufferAndSent(conn, buffer) \
    {                                  \
       conn->SetSendBuffer(buffer);    \
       conn->Write();                  \
    }

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

#undef SetBufferAndSent

}

void HttpConnection::Close()
{
    conn_->Close();
}
