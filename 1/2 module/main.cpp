#include "triangle_area_calculator.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    TriangleAreaCalculator calculator;
    calculator.Run();

    return 0;
}