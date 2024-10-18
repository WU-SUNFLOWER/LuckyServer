#include <iostream>
#include <unistd.h>
#include <string.h>

#include "util.h"
#include "buffer.h"
#include "socket.h"

int main()
{
    util::SetDebuggingMode(true);
    Socket *sock = new Socket();
    InetAddress addr("127.0.0.1", 8888);
    sock->Connect(addr);
    int sockfd = sock->GetFd();
    Buffer *send_buffer = new Buffer();
    Buffer *read_buffer = new Buffer();

    while (true)
    {
        send_buffer->GetLine();
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
                util::DebugPrint("message from server: %s\n", read_buffer->ToStr());
                break;
            }
        }
        read_buffer->Clear();
    }

    delete sock;
    delete send_buffer;
    delete read_buffer;
    return 0;
}