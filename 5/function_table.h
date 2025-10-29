#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include <iostream>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;
using std::fixed;
using std::setprecision;

const int TABLE_WIDTH_X = 10;
const int TABLE_WIDTH_Y = 15;
const int PRECISION_X = 2;
const int PRECISION_Y = 4;

double calculateFunction(double x);
void printFunctionTable(double a, double b, double h);

#endif