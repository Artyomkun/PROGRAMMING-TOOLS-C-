#include "function_table.h"

using std::cin;
using std::sqrt;

double calculateFunction(double x) {
    return sqrt(5 - x * x * x);
}

void printFunctionTable(double intervalStart, double intervalEnd, double stepSize) {
    cout << "Таблица значений функции y = sqrt(5 - x^3)" << endl;
    cout << "----------------------------------------" << endl;
    cout << setw(TABLE_WIDTH_X) << "x" << setw(TABLE_WIDTH_Y) << "y" << endl;
    cout << "----------------------------------------" << endl;
    
    for (double currentX = intervalStart; currentX <= intervalEnd; currentX += stepSize) {
        double expressionValue = 5 - currentX * currentX * currentX;
        
        if (expressionValue < 0) {
            cout << setw(TABLE_WIDTH_X) << fixed << setprecision(PRECISION_X) << currentX;
            cout << setw(TABLE_WIDTH_Y) << "не определена" << endl;
        } else {
            double functionValue = calculateFunction(currentX);
            cout << setw(TABLE_WIDTH_X) << fixed << setprecision(PRECISION_X) << currentX;
            cout << setw(TABLE_WIDTH_Y) << fixed << setprecision(PRECISION_Y) << functionValue << endl;
        }
    }
}

int main() {
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