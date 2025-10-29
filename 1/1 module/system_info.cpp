#include "system_info.h"

void SystemInfo::PrintSystemInfo() {
    std::cout << "System Information:" << std::endl;
    std::cout << "-------------------" << std::endl;
    
    #ifdef _WIN32
        std::cout << "OS: Windows" << std::endl;
    #elif __linux__
        std::cout << "OS: Linux" << std::endl;
    #elif __APPLE__
        std::cout << "OS: macOS" << std::endl;
    #else
        std::cout << "OS: Unknown" << std::endl;
    #endif
    
    std::cout << "Architecture: " << ARCHITECTURE << std::endl;
    std::cout << "Compiler: ";
    
    #ifdef __clang__
        std::cout << "Clang " << __clang_major__ << "." << __clang_minor__;
    #elif __GNUC__
        std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__;
    #elif _MSC_VER
        std::cout << "MSVC " << _MSC_VER;
    #else
        std::cout << "Unknown";
    #endif
    
    std::cout << std::endl;
    std::cout << "-------------------" << std::endl << std::endl;
}