#ifndef LUCKYSERVER_UTIL_H_
#define LUCKYSERVER_UTIL_H_

namespace util
{
    extern void PrintErrorAndExit(const char *kMsg);
    extern void ErrIf(bool condition, const char *kMsg);
} // namespace util

#endif // LUCKYSERVER_UTIL_H_