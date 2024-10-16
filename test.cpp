#include <iostream>
#include <unistd.h>
#include <string.h>
#include <functional>

#include "src/util.h"
#include "src/buffer.h"
#include "src/socket.h"
#include "src/thread_pool.h"
void OneClient(int msgs, int wait)
{
    Socket *sock = new Socket();
    InetAddress addr("127.0.0.1", 8888);
    // sock->setnonblocking(); 客户端使用阻塞式连接比较好，方便简单不容易出错
    sock->Connect(addr);
    int sockfd = sock->GetFd();
    Buffer *send_buffer = new Buffer();
    Buffer *read_buffer = new Buffer();
    sleep(wait);
    int count = 0;
    while (count < msgs)
    {
        send_buffer->SetBuf("I'm client!");
        ssize_t write_bytes = write(sockfd, send_buffer->Cstr(), send_buffer->Size());
        if (write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024]; // 这个buf大小无所谓
        while (true)
        {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0)
            {
                read_buffer->Append(buf, read_bytes);
                already_read += read_bytes;
            }
            else if (read_bytes == 0)
            { // EOF
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if (already_read >= send_buffer->Size())
            {
                printf("count: %d, message from server: %s\n", count++, read_buffer->Cstr());
                break;
            }
        }
        read_buffer->Clear();
    }

    delete sock;
}
int main(int argc, char *argv[])
{
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char *optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1)
    {
        switch (o)
        {
        case 't':
            threads = std::stoi(optarg);
            break;
        case 'm':
            msgs = std::stoi(optarg);
            break;
        case 'w':
            wait = std::stoi(optarg);
            break;
        case '?':
            printf("error optopt: %c\n", optopt);
            printf("error opterr: %d\n", opterr);
            break;
        }
    }
    ThreadPool *poll = new ThreadPool(threads);
    std::function<void()> func = std::bind(OneClient, msgs, wait);
    std::cout << "threads=" << threads << std::endl;
    for (int i = 0; i < threads; ++i)
    {
        poll->AddTask(func);
    }
    delete poll;
    return 0;
}