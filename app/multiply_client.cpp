#include <iostream>
#include <unistd.h>
#include <string.h>
#include <functional>

#include "util.h"
#include "buffer.h"
#include "socket.h"
#include "thread_pool.h"

void OneClient(int msgs, int wait)
{
    Socket *sock = new Socket();
    InetAddress addr("127.0.0.1", 8888);
    
    sock->Connect(addr);
    int sockfd = sock->GetFd();
    Buffer *send_buffer = new Buffer();
    Buffer *read_buffer = new Buffer();
    sleep(wait);
    int count = 0;
    while (count < msgs)
    {
        send_buffer->SetBuf("I'm client!");
        ssize_t write_bytes = write(sockfd, send_buffer->ToStr(), send_buffer->Size());
        if (write_bytes == -1)
        {
            util::DebugPrint("socket already disconnected, can't write any more!\n");
            break;
        }
        size_t already_read = 0;
        char buf[1024];
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
                util::DebugPrint("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if (already_read >= send_buffer->Size())
            {
                util::DebugPrint("count: %d, message from server: %s\n", count++, read_buffer->ToStr());
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
    util::SetDebuggingMode(true);
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
            util::DebugPrint("error optopt: %c\n", optopt);
            util::DebugPrint("error opterr: %d\n", opterr);
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