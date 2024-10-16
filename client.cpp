#include "src/util.h"
#include "src/syscall.h"

int main()
{
    int socket_fd = mysyscall::CreateSocket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr
    {
    };

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    mysyscall::Connect(socket_fd, (sockaddr *)&serv_addr, sizeof(serv_addr));

    char buf[1024];
    while (true)
    {
        // send data to server
        std::memset(buf, 0, sizeof(buf));
        std::scanf("%s", buf);
        ssize_t write_bytes = mysyscall::Write(socket_fd, buf, sizeof(buf));
        util::ErrIf(write_bytes == -1, "socket already disconnected, can't write any more!");
        printf("send %ld bytes message to server: %s\n", write_bytes, buf);

        // read data from server
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(socket_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("read %ld bytes message from server: %s\n", read_bytes, buf);
        }
        else if (read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            mysyscall::Close(socket_fd);
            break;
        }
        else if (read_bytes == -1)
        {
            mysyscall::Close(socket_fd);
            util::PrintErrorAndExit("socket read error");
        }
        else
        {
            mysyscall::Close(socket_fd);
            util::PrintErrorAndExit("unknown return value of read function!");
        }
    }
}