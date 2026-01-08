#include "system_info.h"

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <sys/utsname.h>
#endif

const char* SystemInfo::GetOperatingSystem() {
#ifdef _WIN32
    return "Windows";
#elif __linux__
    return "Linux";
#elif __APPLE__
    return "macOS";
#elif __unix__
    return "Unix";
#else
    return "Unknown OS";
#endif
}

const char* SystemInfo::GetArchitecture() {
#if defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
#elif defined(__i386__) || defined(_M_IX86)
    return "x86";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return "ARM64";
#elif defined(__arm__) || defined(_M_ARM)
    return "ARM";
#else
    return "Unknown Architecture";
#endif
}

const char* SystemInfo::GetCompilerInfo() {
#ifdef __clang__
    return "Clang";
#elif __GNUC__
    return "GCC";
#elif _MSC_VER
    return "MSVC";
#else
    return "Unknown Compiler";
#endif
}

void SystemInfo::PrintSystemInfo() {
    std::cout << "==============================================" << std::endl;
    std::cout << "              SYSTEM INFORMATION" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Operating System: " << GetOperatingSystem() << std::endl;
    std::cout << "Architecture: " << GetArchitecture() << std::endl;
    std::cout << "Compiler: " << GetCompilerInfo() << std::endl;

#ifdef __clang__
    std::cout << "Clang Version: " << __clang_major__ << "." 
              << __clang_minor__ << "." << __clang_patchlevel__ << std::endl;
#elif __GNUC__
    std::cout << "GCC Version: " << __GNUC__ << "." 
              << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << std::endl;
#elif _MSC_VER
    std::cout << "MSVC Version: " << _MSC_VER << std::endl;
#endif
    
    std::cout << "C++ Standard: __cplusplus = " << __cplusplus << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << std::endl;
}