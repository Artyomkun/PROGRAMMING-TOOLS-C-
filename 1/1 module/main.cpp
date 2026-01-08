#include "solver.h"
#include "system_info.h"

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    MathExpressionSolver solver;
    solver.Run();

    return 0;
}