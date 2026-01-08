#include <iostream>
#include <locale> 
#include <clocale>

const size_t MAX_SIZE = 10;

bool has_row_with_only_positive_elements(const int matrix[][MAX_SIZE], size_t n) 
{
    for (size_t i = 0; i < n; ++i) 
    {
        bool all_positive = true;
        
        for (size_t j = 0; j < n; ++j) 
        {
            if (matrix[i][j] <= 0) 
            {
                all_positive = false;
                break;
            }
        }
        
        if (all_positive) 
        {
            return true;
        }
    }
    
    return false;
}

void print_matrix(const int matrix[][MAX_SIZE], size_t n) 
{
    std::cout << "Матрица " << n << "x" << n << ":" << std::endl;
    for (size_t i = 0; i < n; ++i) 
    {
        for (size_t j = 0; j < n; ++j) 
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() 
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    size_t n;
    
    std::cout << "=== Проверка наличия строки только с положительными элементами ===" << std::endl;
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
    
    print_matrix(matrix, n);
    
    bool result = has_row_with_only_positive_elements(matrix, n);
    
    if (result) 
    {
        std::cout << "В матрице ЕСТЬ строка, состоящая только из положительных элементов." << std::endl;
        
        std::cout << "Строки с только положительными элементами: ";
        for (size_t i = 0; i < n; ++i) 
        {
            bool all_positive = true;
            for (size_t j = 0; j < n; ++j) 
            {
                if (matrix[i][j] <= 0) 
                {
                    all_positive = false;
                    break;
                }
            }
            if (all_positive) 
            {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
    }
    else 
    {
        std::cout << "В матрице НЕТ строки, состоящей только из положительных элементов." << std::endl;
    }
    
    return 0;
}