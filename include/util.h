#ifndef LUCKYSERVER_UTIL_H_
#define LUCKYSERVER_UTIL_H_

#include <string>

namespace util
{
    extern void SetDebuggingMode(bool flag);
    extern void DebugPrint(const char *format, ...);
    extern void PrintErrorAndExit(const char *kMsg);
    extern void ErrIf(bool condition, const char *kMsg);
    extern std::string &Trim(std::string &str);
    extern std::string &ToLower(std::string &str);
    extern bool EndsWith(const std::string& str, const std::string &suffix);
    extern std::string GetFilenameSuffix(const std::string& filename);
} // namespace util

#endif // LUCKYSERVER_UTIL_H_