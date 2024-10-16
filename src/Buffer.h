#ifndef HEAD_BUFFER
#define HEAD_BUFFER

#include <string>

class Buffer {
private:
    std::string buf;
public:
    Buffer();
    
    ~Buffer();

    void append(const char* src, size_t size);

    size_t size();

    const char* c_str();

    void clear();

    void getline();

    void setBuf(const char* _buf);
};

#endif