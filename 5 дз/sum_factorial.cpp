#include <iostream>

using namespace std;

double calculate_sum(int k) {
    double sum = 0.0;
    double term = 1.0; 

    for (int n = 1; n <= k; ++n) {
        if (n > 1) {
            term = term / (n - 1) / n;
        }
        sum += term;

        if (term < 1e-15) break;  
    }
    return sum;
}

int main() {
    cout << "Введите натуральное число k: ";
    int k;
    cin >> k;

    if (k <= 0) {
        cout << "Ошибка: введите натуральное число." << endl;
        return 1;
    }

    double result = calculate_sum(k);
    cout.precision(15);
    cout << "Сумма S = " << result << endl;

    return 0;
}
