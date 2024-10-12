#include "util.h"
#include <cstdio>
#include <cstdlib>

void printErrorAndExit(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);    
}

void errif(bool condition, const char* msg) {
    if (condition) {
        printErrorAndExit(msg);
    }
}