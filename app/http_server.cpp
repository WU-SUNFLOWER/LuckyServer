#include <csignal>
#include <cstdio>
#include <cstdlib>

#include "http_server.h"

HttpServer *server = nullptr;

void handle_sigint(int) {
    if (server != nullptr) {
        delete server;
    }
    exit(0);
}

int main() {
    HttpServer *server = new HttpServer();
    signal(SIGINT, &handle_sigint);
    server->Run();
    return 0;
}