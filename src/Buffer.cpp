#include "Buffer.h"
#include <iostream>

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::append(const char *src, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (src[i] == '\0') {
            break;
        }
        buf.push_back(src[i]);
    }
}

size_t Buffer::size() {
    return buf.size();
}

const char *Buffer::c_str() {
    return buf.c_str();
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::getline() {
    buf.clear();
    std::getline(std::cin, buf);
}

void Buffer::setBuf(const char* _buf) {
    buf.clear();
    buf.append(_buf);
}
