#include <iostream>
#include <climits>
#include <locale>
#include <clocale>

const size_t MAX_SIZE = 10;

int calculate_row_sum(const int matrix[][MAX_SIZE], size_t row_index, size_t n) 
{
    int sum = 0;
    for (size_t j = 0; j < n; ++j) 
    {
        sum += matrix[row_index][j];
    }
    return sum;
}

int find_max_element(const int array[], size_t size) 
{
    int max_element = INT_MIN;
    for (size_t i = 0; i < size; ++i) 
    {
        if (array[i] > max_element) 
        {
            max_element = array[i];
        }
    }
    return max_element;
}

int main() 
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    size_t n;
    
    std::cout << "=== Суммы строк матрицы и поиск максимального элемента ===" << std::endl;
    std::cout << "Введите размер квадратной матрицы n (не более " << MAX_SIZE << "): ";
    std::cin >> n;
    
    if (n <= 0 || n > MAX_SIZE) 
    {
        std::cout << "Ошибка: некорректный размер матрицы!" << std::endl;
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    std::cout << "Введите элементы матрицы " << n << "x" << n << ":" << std::endl;
    
    for (size_t i = 0; i < n; ++i) 
    {
        std::cout << "Строка " << i << ": ";
        for (size_t j = 0; j < n; ++j) 
        {
            std::cin >> matrix[i][j];
        }
    }
    
    std::cout << "\nИсходная матрица " << n << "x" << n << ":" << std::endl;
    for (size_t i = 0; i < n; ++i) 
    {
        for (size_t j = 0; j < n; ++j) 
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    
    int row_sums[MAX_SIZE];
    std::cout << "\nСуммы элементов каждой строки:" << std::endl;
    
    for (size_t i = 0; i < n; ++i) 
    {
        row_sums[i] = calculate_row_sum(matrix, i, n);
        std::cout << "Строка " << i << ": сумма = " << row_sums[i] << std::endl;
    }
    
    std::cout << "\nНовый массив (суммы строк):" << std::endl;
    std::cout << "[ ";
    for (size_t i = 0; i < n; ++i) 
    {
        std::cout << row_sums[i];
        if (i < n - 1) 
        {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;
    
    int max_element = find_max_element(row_sums, n);
    std::cout << "\nМаксимальный элемент нового массива: " << max_element << std::endl;
    
    return 0;
}