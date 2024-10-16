#ifndef HEAD_BUFFER
#define HEAD_BUFFER

#include <string>

class Buffer {
private:
    std::string buf_;
public:
    Buffer();
    
    ~Buffer();

    void append(const char* kSrc, size_t size);

    size_t size();

    const char* c_str();

    void clear();

    void getline();
};

#endif