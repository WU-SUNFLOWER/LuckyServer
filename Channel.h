#ifndef HEAD_CHANNEL
#define HEAD_CHANNEL

#include <cstdint>

class Epoll;

class Channel {
private:
    Epoll* epoll;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
public:

    Channel(Epoll* _epoll, int _fd);
    ~Channel();

    void enableReading();

    int getFd();

    uint32_t getEvents();

    uint32_t getRevents();

    bool getInEpoll();

    void setInEpoll();

    void setRevents(uint32_t _revents);

};

#endif