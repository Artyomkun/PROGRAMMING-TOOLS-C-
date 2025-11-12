#include <iostream>
#include <cmath>
#include <utility>  

using namespace std;

pair<double, int> calculate_series_sum(double epsilon) {
    double sum = 0.0;
    double term;
    int i = 1;

    do {
        term = pow(-2, i + 1) / (i * (2 * i + 1));
        sum += term;
        ++i;
    } while (fabs(term) >= epsilon);

    return make_pair(sum, i - 1);
}

int main() {
    cout << "Введите точность epsilon (e > 0): ";
    double epsilon_value;
    cin >> epsilon_value;

    if (epsilon_value <= 0) {
        cout << "Ошибка: epsilon должно быть положительным числом." << endl;
        return 1;
    }

    auto [series_sum, terms_count] = calculate_series_sum(epsilon_value);

    cout.precision(10);
    cout << "Сумма ряда с точностью " << epsilon_value << " равна: " << series_sum << endl;
    cout << "Количество просуммированных членов: " << terms_count << endl;

    return 0;
}
