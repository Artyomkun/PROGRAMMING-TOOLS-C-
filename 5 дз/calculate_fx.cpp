#include <iostream>
#include <cmath>
#include <utility>
#include <iomanip>

using namespace std;

pair<double, int> calculate_Fx(const double* px, const double* pepsilon) {
    double x = *px;
    double epsilon = *pepsilon;

    double sum = 1.0;
    double term = -pow(x, 3) / (3 * pow(4, 2));
    int count = 2;

    sum += term;

    int power = 5;
    int den_left = 4;
    int den_right = 5;
    int sign = 1;

    while (fabs(term) >= epsilon) {
        term = sign * pow(x, power) / (den_left * pow(den_right, 2));
        sum += term;

        sign = -sign;
        power += 2;
        den_left++;
        den_right++;
        count++;
    }
    return make_pair(sum, count);
}

int main() {
    const double a = 0.0;
    const double b = 0.8;
    const double h = 0.1;

    cout << "Введите точность epsilon (e > 0): ";
    double epsilon;
    cin >> epsilon;

    if (epsilon <= 0) {
        cout << "Ошибка: epsilon должно быть положительным числом." << endl;
        return 1;
    }

    cout 
        << left << setw(30) << "Значение x"
        << left << setw(25) << "Значение F(x)"
        << left << setw(25) << "Количество слагаемых" << endl;

    int index = 1;
    for (double x = a; x <= b + 1e-9; x += h) {
        auto [value, terms] = calculate_Fx(&x, &epsilon);
        cout 
            << left << setw(25) << fixed << setprecision(2) << x
            << left << setw(25) << setprecision(10) << value
            << left << setw(25) << terms << endl;
        ++index;
    }

    return 0;
}
