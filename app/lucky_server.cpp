#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <filesystem>

#include "http_server.h"

HttpServer *server = nullptr;

void handle_sigint(int) {
    if (server != nullptr) {
        delete server;
    }
    exit(0);
}

int main(int argc, char** argv) {

    int option = 0;
    int user_port = 8888;
    std::string user_path_str = "";
    while ((option = getopt(argc, argv, "d:p:")) != -1) {
        switch (option) {
            case 'd':
                user_path_str = optarg;
                break;
            case 'p':
                user_port = std::stoi(optarg);
                break;
            case '?':
                printf("error user option: %c\n", (char)optopt);
                break;
        }
    }

    std::filesystem::path work_dir = std::filesystem::current_path();
    if (!user_path_str.empty()) {
        std::filesystem::path user_path = user_path_str;
        if (user_path.is_absolute()) {
            work_dir = user_path;
        } else {
            work_dir = work_dir / user_path;
        }
    }

    HttpServer *server = new HttpServer(user_port, work_dir);
    signal(SIGINT, &handle_sigint);
    server->Run();
    return 0;
}