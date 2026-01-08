#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

namespace FunctionTable {
    const int TABLE_WIDTH_X = 12;
    const int TABLE_WIDTH_Y = 18;
    const int PRECISION_X = 2;
    const int PRECISION_Y = 4;

    double calculateFunction(double x);
    void printFunctionTable(double a, double b, double h);
}

#endif