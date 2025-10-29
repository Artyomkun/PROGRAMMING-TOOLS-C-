#ifndef PIECEWISE_FUNCTION_H
#define PIECEWISE_FUNCTION_H

#include <iostream>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;
using std::cin;
using std::setw;
using std::fixed;
using std::setprecision;
using std::fabs;

const double BOUNDARY_1 = 0.1;
const double BOUNDARY_2 = 0.2;
const double CONSTANT_1 = 0.1;
const double CONSTANT_2 = 0.2;

double calculatePiecewiseFunction(double x);
void printFunctionTable(double start, double end, double step);

#endif