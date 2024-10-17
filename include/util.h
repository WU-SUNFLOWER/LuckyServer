#ifndef LUCKYSERVER_UTIL_H_
#define LUCKYSERVER_UTIL_H_

#include <string>

namespace util
{
    extern void PrintErrorAndExit(const char *kMsg);
    extern void ErrIf(bool condition, const char *kMsg);
    extern std::string &Trim(std::string &str);
    extern std::string &ToLower(std::string &str);
} // namespace util

#endif // LUCKYSERVER_UTIL_H_