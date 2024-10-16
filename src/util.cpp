#include "util.h"
#include <cstdio>
#include <cstdlib>

void printErrorAndExit(const char* kMsg) {
    perror(kMsg);
    exit(EXIT_FAILURE);    
}

void errif(bool condition, const char* kMsg) {
    if (condition) {
        printErrorAndExit(kMsg);
    }
}