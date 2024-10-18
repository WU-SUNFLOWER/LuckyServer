#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cctype>

namespace util
{
    void PrintErrorAndExit(const char *kMsg)
    {
        perror(kMsg);
        exit(EXIT_FAILURE);
    }

    void ErrIf(bool condition, const char *kMsg)
    {
        if (condition)
        {
            PrintErrorAndExit(kMsg);
        }
    }

    std::string &Trim(std::string &str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](auto ch) {
            return !std::isspace(ch);
        }));
        str.erase(std::find_if(str.rbegin(), str.rend(), [](auto ch) {
            return !std::isspace(ch);
        }).base(), str.end());
        return str;
    }

    std::string &ToLower(std::string &str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](auto ch) {
            return std::tolower(ch);
        });
        return str;
    }

    bool EndsWith(const std::string& str, const std::string &suffix) {
        if (str.length() < suffix.length()) {
            return false;
        }
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix);
    }

    std::string GetFilenameSuffix(const std::string& filename) {
        size_t pos = filename.rfind('.');
        if (pos == std::string::npos) {
            return "";
        }
        return filename.substr(pos + 1);
    }
} // namespace util