#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <numeric>
#include <functional>

// ============================================================================
// КОНСТАНТЫ
// ============================================================================

const size_t DEMONSTRATION_SEQUENCE_SIZE = 5;

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ============================================================================

std::string format_number(double number)
{
    std::stringstream stream;
    
    if (number == static_cast<int>(number))
    {
        stream << static_cast<int>(number);
    }
    else
    {
        stream << std::fixed << std::setprecision(2) << number;
    }
    
    return stream.str();
}

// Функтор для форматирования с состоянием
class FormatterWithState {
private:
    size_t current_index;
    size_t total_size;
    
public:
    FormatterWithState(size_t size) : current_index(0), total_size(size) {}
    
    std::string operator()(double value) {
        std::string result = format_number(value);
        if (current_index++ < total_size - 1) {
            result += " ";
        }
        return result;
    }
};

// ============================================================================
// ОСНОВНЫЕ ФУНКЦИИ ПРОГРАММЫ
// ============================================================================

void double_negative_elements(std::vector<double>& numbers)
{
    std::transform(numbers.begin(), numbers.end(), numbers.begin(), 
        [](double value) -> double {
            return (value < 0.0) ? value * 2.0 : value;
        });
}

std::vector<double> input_sequence()
{
    std::cout << "Введите последовательность чисел через пробел: ";
    
    std::string input_line;
    std::getline(std::cin, input_line);
    
    std::vector<double> sequence;
    std::istringstream stream(input_line);
    
    std::copy(std::istream_iterator<double>(stream), std::istream_iterator<double>(), std::back_inserter(sequence));
    
    return sequence;
}

void output_sequence(const std::vector<double>& sequence, const std::string& title = "")
{
    if (sequence.empty())
    {
        std::cout << "Последовательность пуста." << std::endl;
        return;
    }
    
    if (!title.empty())
    {
        std::cout << title << ": ";
    }
    
    FormatterWithState formatter(sequence.size());
    std::transform(sequence.begin(), sequence.end(), std::ostream_iterator<std::string>(std::cout), formatter);
    
    std::cout << std::endl;
}

size_t count_negative_elements(const std::vector<double>& sequence)
{
    return std::count_if(sequence.begin(), sequence.end(), [](double value) { 
        return value < 0.0; 
    });
}

// ============================================================================
// ФУНКЦИИ ДЛЯ ДЕМОНСТРАЦИИ АЛГОРИТМОВ STL
// ============================================================================

void demonstrate_min_max(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    auto result = std::minmax_element(sequence.begin(), sequence.end());
    
    std::cout << "  Минимум: " << format_number(*result.first) 
                << " (позиция: " << std::distance(sequence.begin(), result.first) + 1 << ")" << std::endl;
    std::cout << "  Максимум: " << format_number(*result.second) 
                << " (позиция: " << std::distance(sequence.begin(), result.second) + 1 << ")" << std::endl;
}

void demonstrate_accumulate(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    double sum = std::accumulate(sequence.begin(), sequence.end(), 0.0);
    std::cout << "  Сумма: " << format_number(sum) << std::endl;
    
    double average = sum / sequence.size();
    std::cout << "  Среднее: " << format_number(average) << std::endl;
}

void demonstrate_logical_checks(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    bool has_positive = std::any_of(sequence.begin(), sequence.end(),[](double value) { 
        return value > 0.0; 
    });
    
    bool all_non_negative = std::all_of(sequence.begin(), sequence.end(),[](double value) { 
        return value >= 0.0; 
    });
    
    bool none_zero = std::none_of(sequence.begin(), sequence.end(), [](double value) { 
        return value == 0.0; 
    });
    
    std::cout << "  Есть положительные: " << (has_positive ? "Да" : "Нет") << std::endl;
    std::cout << "  Все неотрицательные: " << (all_non_negative ? "Да" : "Нет") << std::endl;
    std::cout << "  Нет нулей: " << (none_zero ? "Да" : "Нет") << std::endl;
}

// Демонстрация алгоритма поиска
void demonstrate_find(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    auto first_negative = std::find_if(sequence.begin(), sequence.end(),[](double value) { 
        return value < 0.0; 
    });
    
    auto first_positive = std::find_if(sequence.begin(), sequence.end(), [](double value) { 
        return value > 0.0; 
    });
    
    if (first_negative != sequence.end())
    {
        std::cout << "  Первый отрицательный: " << format_number(*first_negative)
                  << " (позиция: " << std::distance(sequence.begin(), first_negative) + 1 << ")" << std::endl;
    }
    
    if (first_positive != sequence.end())
    {
        std::cout << "  Первый положительный: " << format_number(*first_positive)
                  << " (позиция: " << std::distance(sequence.begin(), first_positive) + 1 << ")" << std::endl;
    }
}

void demonstrate_reverse(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    std::vector<double> reversed = sequence;
    std::reverse(reversed.begin(), reversed.end());
    
    std::cout << "  Реверс: ";
    output_sequence(reversed, "");
}

void demonstrate_copy_if(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    std::vector<double> positive_numbers;
    
    std::copy_if(sequence.begin(), sequence.end(),
                 std::back_inserter(positive_numbers),
                 [](double value) { return value > 0.0; });
    
    std::cout << "  Только положительные: ";
    output_sequence(positive_numbers, "");
}

void demonstrate_sort(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    std::vector<double> sorted = sequence;
    std::sort(sorted.begin(), sorted.end());
    
    std::cout << "  Отсортировано по возрастанию: ";
    output_sequence(sorted, "");
}

void demonstrate_unique(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    std::vector<double> unique_sorted = sequence;
    std::sort(unique_sorted.begin(), unique_sorted.end());
    
    auto last = std::unique(unique_sorted.begin(), unique_sorted.end());
    unique_sorted.erase(last, unique_sorted.end());
    
    std::cout << "  Уникальные элементы: ";
    output_sequence(unique_sorted, "");
}

void demonstrate_partition(const std::vector<double>& sequence)
{
    if (sequence.empty()) return;
    
    std::vector<double> partitioned = sequence;
    
    auto boundary = std::partition(partitioned.begin(), partitioned.end(),
        [](double value) { return value < 0.0; });
    
    std::cout << "  После partition (сначала отрицательные): ";
    output_sequence(partitioned, "");
    
    size_t negative_count = std::distance(partitioned.begin(), boundary);
    std::cout << "  Количество отрицательных: " << negative_count << std::endl;
}

// ============================================================================
// ГЛАВНАЯ ФУНКЦИЯ
// ============================================================================

int main()
{
    system("chcp 65001 > nul");
    
    std::cout << "===================================================" << std::endl;
    std::cout << "   УДВОЕНИЕ ОТРИЦАТЕЛЬНЫХ ЧИСЕЛ" << std::endl;
    std::cout << "   (использование стандартных алгоритмов STL)" << std::endl;
    std::cout << "===================================================\n" << std::endl;
    
    std::vector<double> numbers = input_sequence();
    
    if (numbers.empty())
    {
        std::cout << "\nОшибка: не введено чисел или ввод пуст." << std::endl;
        std::cout << "Программа завершена." << std::endl;
        return 1;
    }
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ИСХОДНЫЕ ДАННЫЕ:" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    output_sequence(numbers, "Последовательность");
    
    size_t negative_count = count_negative_elements(numbers);
    std::cout << "Количество отрицательных элементов: " << negative_count << std::endl;
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ОБРАБОТКА ДАННЫХ:" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "Выполняется удвоение отрицательных элементов..." << std::endl;
    double_negative_elements(numbers);
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "РЕЗУЛЬТАТЫ ОБРАБОТКИ:" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    output_sequence(numbers, "Обработанная последовательность");
    
    size_t new_negative_count = count_negative_elements(numbers);
    std::cout << "Количество отрицательных элементов после обработки: "  
                << new_negative_count << std::endl;
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ДЕМОНСТРАЦИЯ СТАНДАРТНЫХ АЛГОРИТМОВ STL:" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "\n1. Поиск минимума и максимума:" << std::endl;
    demonstrate_min_max(numbers);
    
    std::cout << "\n2. Сумма и среднее:" << std::endl;
    demonstrate_accumulate(numbers);
    
    std::cout << "\n3. Логические проверки:" << std::endl;
    demonstrate_logical_checks(numbers);
    
    std::cout << "\n4. Поиск элементов:" << std::endl;
    demonstrate_find(numbers);
    
    std::cout << "\n5. Реверс последовательности:" << std::endl;
    demonstrate_reverse(numbers);
    
    std::cout << "\n6. Копирование с условием:" << std::endl;
    demonstrate_copy_if(numbers);
    
    std::cout << "\n7. Сортировка:" << std::endl;
    demonstrate_sort(numbers);
    
    std::cout << "\n8. Удаление дубликатов:" << std::endl;
    demonstrate_unique(numbers);
    
    std::cout << "\n9. Разделение (partition):" << std::endl;
    demonstrate_partition(numbers);
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ПРОГРАММА ЗАВЕРШЕНА УСПЕШНО" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    return 0;
}