#include "piecewise_function.h"

double calculatePiecewiseFunction(double x) {
    double absoluteX = fabs(x);
    
    if (absoluteX <= BOUNDARY_1) {
        return x * x * x - CONSTANT_1;
    } else if (absoluteX <= BOUNDARY_2) {
        return CONSTANT_2 * x - CONSTANT_1;
    } else {
        return x * x * x + CONSTANT_1;
    }
}

void printFunctionTable(double start, double end, double step) {
    cout << "Таблица значений кусочной функции:" << endl;
    cout << "y = x³ - 0.1, если |x| ≤ 0.1" << endl;
    cout << "y = 0.2x - 0.1, если 0.1 < |x| ≤ 0.2" << endl;
    cout << "y = x³ + 0.1, если |x| > 0.2" << endl;
    cout << "----------------------------------------" << endl;
    cout << setw(12) << "x" << setw(15) << "y" << endl;
    cout << "----------------------------------------" << endl;
    
    for (double currentX = start; currentX <= end; currentX += step) {
        double resultY = calculatePiecewiseFunction(currentX);
        cout << setw(12) << fixed << setprecision(3) << currentX;
        cout << setw(15) << fixed << setprecision(4) << resultY << endl;
    }
}

int main() {
    double intervalStart;
    double intervalEnd; 
    double stepSize;
    
    cout << "=== Построение таблицы кусочной функции ===" << endl;
    cout << "Введите начало интервала: ";
    cin >> intervalStart;
    
    cout << "Введите конец интервала: ";
    cin >> intervalEnd;
    
    cout << "Введите шаг: ";
    cin >> stepSize;
    
    cout << endl;
    printFunctionTable(intervalStart, intervalEnd, stepSize);
    
    return 0;
}