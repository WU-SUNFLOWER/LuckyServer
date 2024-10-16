#ifndef HEAD_UTIL
#define HEAD_UTIL

namespace util{
void PrintErrorAndExit(const char *kMsg);
void ErrIf(bool condition, const char *kMsg);
}

#endif