#include "src/EventLoop.h"
#include "src/Server.h"

int main() {

    EventLoop loop;
    Server server(&loop);
    loop.loop();
    
    return 0;
}