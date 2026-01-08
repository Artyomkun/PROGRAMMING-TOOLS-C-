#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <iostream>

#if defined(__x86_64__) || defined(_M_X64)
    #define ARCHITECTURE "x86_64"
#elif defined(__i386__) || defined(_M_IX86)
    #define ARCHITECTURE "x86"
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define ARCHITECTURE "ARM64"
#elif defined(__arm__) || defined(_M_ARM)
    #define ARCHITECTURE "ARM"
#else
    #define ARCHITECTURE "Unknown"
#endif

class SystemInfo {
public:
    static void PrintSystemInfo();
};

#endif