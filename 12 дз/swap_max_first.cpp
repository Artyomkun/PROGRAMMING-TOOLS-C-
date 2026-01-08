#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <numeric>

// ============================================================================
// КОНСТАНТЫ
// ============================================================================

const char* const PROGRAM_TITLE = "ОБМЕН МАКСИМАЛЬНОГО ЭЛЕМЕНТА С ПЕРВЫМ";

// ============================================================================
// ОСНОВНЫЕ ФУНКЦИИ ПРОГРАММЫ
// ============================================================================

// Функция для обмена максимального элемента с первым
void swap_max_with_first(std::vector<int>& numbers)
{
    if (numbers.empty())
    {
        return;
    }

    auto max_element_iterator = std::max_element(numbers.begin(), numbers.end());
    std::iter_swap(numbers.begin(), max_element_iterator);
}

int input_element_count()
{
    int element_count;
    
    std::cout << "Введите количество элементов в массиве: ";
    std::cin >> element_count;
    
    return element_count;
}

std::vector<int> input_numbers(int element_count)
{
    std::vector<int> numbers(element_count);
    
    if (element_count > 0)
    {
        std::cout << "Введите " << element_count << " целых чисел:" << std::endl;
        
        std::for_each(numbers.begin(), numbers.end(), [](int& element) {
            static int counter = 1;
            std::cout << "Элемент " << counter++ << ": ";
            std::cin >> element;
        });
    }
    
    return numbers;
}

void output_array(const std::vector<int>& array, const std::string& message)
{
    std::cout << message << ": ";
    
    std::copy(array.begin(), array.end(), std::ostream_iterator<int>(std::cout, " "));
    
    std::cout << std::endl;
}

// ============================================================================
// ГЛАВНАЯ ФУНКЦИЯ
// ============================================================================

int main()
{
    system("chcp 65001 > nul");
    
    // Заголовок программы
    std::cout << "==========================================" << std::endl;
    std::cout << "   " << PROGRAM_TITLE << std::endl;
    std::cout << "==========================================\n" << std::endl;
    
    int element_count = input_element_count();
    
    if (element_count <= 0)
    {
        std::cout << "Ошибка: количество элементов должно быть положительным числом." << std::endl;
        return 1;
    }
    
    std::vector<int> numbers = input_numbers(element_count);
    
    std::cout << "\n" << std::string(40, '-') << std::endl;
    output_array(numbers, "Исходный массив");
    
    std::cout << "\nВыполняется обмен максимального элемента с первым..." << std::endl;
    swap_max_with_first(numbers);
    
    std::cout << "\n" << std::string(40, '-') << std::endl;
    output_array(numbers, "Массив после обмена");
    
    std::cout << "\n" << std::string(40, '-') << std::endl;
    std::cout << "ИНФОРМАЦИЯ О МАССИВЕ:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    if (!numbers.empty())
    {
        auto new_max_iterator = std::max_element(numbers.begin(), numbers.end());
        size_t max_position = std::distance(numbers.begin(), new_max_iterator);
        
        auto min_iterator = std::min_element(numbers.begin(), numbers.end());
        size_t min_position = std::distance(numbers.begin(), min_iterator);
        
        int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
        
        std::cout << "Размер массива: " << numbers.size() << " элементов" << std::endl;
        std::cout << "Максимальный элемент: " << *new_max_iterator 
                    << " (позиция: " << max_position + 1 << ")" << std::endl;
        std::cout << "Минимальный элемент: " << *min_iterator 
                    << " (позиция: " << min_position + 1 << ")" << std::endl;
        std::cout << "Сумма всех элементов: " << sum << std::endl;
        
        int even_count = std::count_if(numbers.begin(), numbers.end(), [](int value) { 
            return value % 2 == 0; 
        });

        int odd_count = numbers.size() - even_count;
        
        std::cout << "Четных элементов: " << even_count << std::endl;
        std::cout << "Нечетных элементов: " << odd_count << std::endl;
    }
    
    // Завершение программы
    std::cout << "\n" << std::string(40, '=') << std::endl;
    std::cout << "ПРОГРАММА ЗАВЕРШЕНА УСПЕШНО" << std::endl;
    std::cout << std::string(40, '=') << std::endl;
    
    return 0;
}