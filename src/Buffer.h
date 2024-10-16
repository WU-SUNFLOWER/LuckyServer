#ifndef LUCKYSERVER_BUFFER_H_
#define LUCKYSERVER_BUFFER_H_

#include <string>

class Buffer
{
private:
    std::string buf_;

public:
    Buffer();

    ~Buffer();

    void Append(const char *kSrc, size_t size);

    size_t Size() const;

    const char *Cstr();

    void Clear();

    void Getline();
};

#endif // LUCKYSERVER_BUFFER_H_