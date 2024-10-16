#ifndef HEAD_BUFFER
#define HEAD_BUFFER

#include <string>

class Buffer {
private:
    std::string buf_;
public:
    Buffer();
    
    ~Buffer();

    void Append(const char* kSrc, size_t size);

    size_t Size();

    const char* C_str();

    void Clear();

    void Getline();
};

#endif