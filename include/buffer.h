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

    const std::string &ToRaw();

    void Clear();

    void GetLine();

    void SetBuf(const char *buf);

    void SetBuf(const char *buf, size_t n);
};

#endif // LUCKYSERVER_BUFFER_H_