#include "syscall.h"
#include "util.h"
#include <errno.h>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setNonBlocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    int socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    Bind(socket_fd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    Listen(socket_fd, SOMAXCONN);

    int epoll_fd = EpollCreate1(0);
    struct epoll_event events[MAX_EVENTS];
    struct epoll_event event_config;

    bzero(&events, sizeof(events));
    bzero(&event_config, sizeof(event_config));

    event_config.data.fd = socket_fd;
    event_config.events = EPOLLIN | EPOLLET;
    setNonBlocking(socket_fd);
    EpollCtl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event_config);

    char buf[READ_BUFFER];
    while (true) {
        int total_fd = EpollWait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < total_fd; ++i) {
            struct epoll_event& cur_event = events[i];
            if (cur_event.data.fd == socket_fd) {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                bzero(&client_addr, sizeof(client_addr));

                int client_socket_fd = Accept(socket_fd, (sockaddr*)&client_addr, &client_addr_len);
                printf("New client fd %d! From IP: %s Port: %d\n", 
                        client_socket_fd, 
                        inet_ntoa(client_addr.sin_addr), 
                        ntohs(client_addr.sin_port)
                );

                bzero(&event_config, sizeof(event_config));
                event_config.data.fd = client_socket_fd;
                event_config.events = EPOLLIN | EPOLLET;
                setNonBlocking(client_socket_fd);
                EpollCtl(epoll_fd, EPOLL_CTL_ADD, client_socket_fd, &event_config);
            }
            else if (cur_event.events & EPOLLIN) {
                int client_socket_fd = cur_event.data.fd;
                while (true) {
                    bzero(buf, sizeof(buf));
                    ssize_t read_bytes = read(client_socket_fd, buf, sizeof(buf));
                    if (read_bytes > 0) {
                        printf("read %ld bytes mssage from client fd %d: %s\n", read_bytes, client_socket_fd, buf);
                        write(client_socket_fd, buf, sizeof(buf));
                    } 
                    else if (read_bytes == 0) {
                        printf("client fd %d disconnected\n", client_socket_fd);
                        close(client_socket_fd);
                        break;
                    } 
                    else if (read_bytes == -1 && errno == EINTR) {
                        printf("continue reading...\n");
                        continue;
                    } 
                    else if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    } 
                    else {
                        close(client_socket_fd);
                        printErrorAndExit("unknown return value of read function.");
                    }
                }
            }
            else {
                printf("something else happened\n");
            }
        }
    }

    close(socket_fd);
    return 0;
}