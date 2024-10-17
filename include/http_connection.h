#ifndef LUCKYSERVER_HTTP_CONNECTION_H_
#define LUCKYSERVER_HTTP_CONNECTION_H_

#include <string>
#include <unordered_map>

class Connection;

class HttpConnection
{
private:
    Connection *conn_;

    std::string method_;
    std::string uri_;
    std::string http_version_;

    std::unordered_map<std::string, std::string> headers_;

public:
    HttpConnection(Connection *conn);

    ~HttpConnection();

    const std::string &GetMethod();
    const std::string &GetURI();
    const std::string &GetHttpVersion();

    bool HasHeader(const std::string &key);
    const std::string &GetHeader(const std::string &key);

    bool ReadRequestMessage();

    bool ParseRequestMessage();

    // debug only!!!
    void PrintRequestMessage();

    std::string ParseURI();

    void RespondSimply(const char *cause, const char *errnum, 
        const char *shortmsg, const char *longmsg);

    void Close();
};

#endif