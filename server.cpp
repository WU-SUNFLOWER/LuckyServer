#include "event_loop.h"
#include "server.h"

int main() {

    EventLoop loop;
    Server server(&loop);
    loop.Loop();
    
    return 0;
}