#include <iostream>
#include <cmath>
#include <locale> 
#include <clocale>

const size_t MAX_SIZE = 100;

int count_pairs_with_given_difference(const double array[], size_t size, double target_difference) 
{
    int count = 0;
    
    for (size_t i = 0; i < size - 1; ++i) 
    {
        if (std::abs(array[i] - array[i + 1]) == target_difference) 
        {
            ++count;
        }
    }
    
    return count;
}

int main() 
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    size_t n;
    double target_diff;
    
    std::cout << "=== Подсчёт пар соседних элементов с заданной разностью ===" << std::endl;
    std::cout << "Введите количество действительных чисел n (не более " << MAX_SIZE << "): ";
    std::cin >> n;
    
    if (n <= 0 || n > MAX_SIZE) 
    {
        std::cout << "Ошибка: некорректный размер массива!" << std::endl;
        return 1;
    }
    std::cout << "Введите заданную разность: ";
    std::cin >> target_diff;

    double numbers[MAX_SIZE];
    std::cout << "Введите " << n << " действительных чисел:" << std::endl;
    
    for (size_t i = 0; i < n; ++i) 
    {
        std::cout << "Элемент [" << i << "]: ";
        std::cin >> numbers[i];
    }
    
    int pair_count = count_pairs_with_given_difference(numbers, n, target_diff);
    
    std::cout << "Количество пар соседних элементов с разностью " << target_diff 
                << " равно: " << pair_count << std::endl;
    
    if (pair_count > 0) 
    {
        std::cout << "Пары соседних элементов с разностью " << target_diff << ":" << std::endl;
        for (size_t i = 0; i < n - 1; ++i) 
        {
            if (std::abs(numbers[i] - numbers[i + 1]) == target_diff) 
            {
                std::cout << "(" << numbers[i] << ", " << numbers[i + 1] << ")" << std::endl;
            }
        }
    }
    
    return 0;
}