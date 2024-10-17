#include "socket.h"
#include "event_loop.h"
#include "server.h"
#include "connection.h"
#include <cstdio>

int main() {

    EventLoop loop;
    Server server(&loop);
    server.OnConnect([](Connection *conn) {
        conn->Read();
        if (conn->GetState() == Connection::State::Closed) {
            conn->Close();
            return;
        }
        printf("message from client %d: %s\n", conn->GetSocket()->GetFd(), conn->ReadBuffer());
        conn->SetSendBuffer(conn->ReadBuffer());
        conn->Write();
        if (conn->GetState() == Connection::State::Closed) {
            conn->Close();
            return;
        }

    });

    loop.Loop();
    
    return 0;
}


