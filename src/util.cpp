#include "util.h"

#include <cstdio>
#include <cstdlib>

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
} // namespace util