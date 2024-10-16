#include "buffer.h"
#include <iostream>

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::Append(const char *kSrc, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (kSrc[i] == '\0')
        {
            break;
        }
        buf_.push_back(kSrc[i]);
    }
}

size_t Buffer::Size()
{
    return buf_.size();
}

const char *Buffer::C_str()
{
    return buf_.c_str();
}

void Buffer::Clear()
{
    buf_.clear();
}

void Buffer::Getline()
{
    buf_.clear();
    std::getline(std::cin, buf_);
}

void Buffer::setBuf(const char* _buf) {
    buf.clear();
    buf.append(_buf);
}
