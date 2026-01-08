#ifndef MATH_SOLVER_H
#define MATH_SOLVER_H

#include <iostream>
#include <cmath>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
#endif

class MathExpressionSolver {
private:
    double x_, y_;
    double result_;

public:
    MathExpressionSolver();
    void ClearScreen();
    void PrintProgramHeader();
    void GetUserInput();
    bool ValidateInput();
    void Calculate();
    void DisplayCalculationResult();
    void DisplayErrorMessage(const char* error_message);
    bool AskForAnotherCalculation();
    void Run();
};

#endif 