#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <iostream>

class SystemInfo {
public:
    static void PrintSystemInfo();
    
private:
    static const char* GetOperatingSystem();
    static const char* GetArchitecture();
    static const char* GetCompilerInfo();
};

#endif 