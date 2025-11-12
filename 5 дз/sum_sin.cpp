#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double calculate_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double rad = i * M_PI / 180.0;
        sum += 1 / sin(rad);
    }
    return sum;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    cout << "Введите натуральное число n: ";
    int n;
    cin >> n;

    if (n <= 0) {
        cout << "Ошибка: n должно быть натуральным числом больше 0." << endl;
        return 1;
    }

    double result = calculate_sum(n);
    cout << "Сумма S = " << result << endl;

    return 0;
}
