#include "function_table.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <locale>
using namespace std;
using namespace FunctionTable;

double FunctionTable::calculateFunction(double x) {
    double expressionValue = 5 - x * x * x;
    
    if (expressionValue < 0) {
        return numeric_limits<double>::quiet_NaN();
    }
    
    return sqrt(expressionValue);
}

void FunctionTable::printFunctionTable(double intervalStart, double intervalEnd, double stepSize) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << "Таблица значений функции y = sqrt(5 - x^3)" << endl;
    cout << "--------------------------------------------" << endl;
    cout << setw(12) << "x" << setw(20) << "y" << endl;
    cout << "--------------------------------------------" << endl;
    
    for (double currentX = intervalStart; currentX <= intervalEnd; currentX += stepSize) {
        double functionValue = calculateFunction(currentX);
        
        cout << setw(12) << fixed << setprecision(PRECISION_X) << currentX;
        
        if (isnan(functionValue)) {
            cout << setw(20) << "               не определена" << endl;
        } else {
            cout << setw(20) << fixed << setprecision(PRECISION_Y) << functionValue << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    double intervalStart;
    double intervalEnd;
    double stepSize;
    
    cout << "Введите начало интервала a: ";
    cin >> intervalStart;
    
    cout << "Введите конец интервала b: ";
    cin >> intervalEnd;
    
    cout << "Введите шаг h: ";
    cin >> stepSize;
    
    cout << endl;
    printFunctionTable(intervalStart, intervalEnd, stepSize);
    
    return 0;
}