#include "src/event_loop.h"
#include "src/server.h"

int main() {

    EventLoop loop;
    Server server(&loop);
    loop.Loop();
    
    return 0;
}