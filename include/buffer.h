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

    const char *ToStr();

    void Clear();

    void GetLine();

    void SetBuf(const char *buf);
};

#endif // LUCKYSERVER_BUFFER_H_