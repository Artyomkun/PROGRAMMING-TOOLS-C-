#include <iostream>
#include <utility>
#include <locale> 
#include <clocale> 

const size_t MAX_SIZE = 100;
const size_t MAX_ROWS = 10;
const size_t MAX_COLS = 10;

bool is_three_digit_number(int number) 
{
    return number >= 100 && number <= 999;
}

bool check_product_one_dimensional(const int array[], size_t size) 
{
    long long product = 1;
    
    for (size_t i = 0; i < size; ++i) 
    {
        product *= array[i];
    }
    
    return is_three_digit_number(static_cast<int>(product));
}

bool check_product_two_dimensional(const int matrix[][MAX_COLS], size_t rows, size_t cols) 
{
    long long product = 1;
    
    for (size_t i = 0; i < rows; ++i) 
    {
        for (size_t j = 0; j < cols; ++j) 
        {
            product *= matrix[i][j];
        }
    }
    
    return is_three_digit_number(static_cast<int>(product));
}

void input_one_dimensional_array() 
{
    size_t size;
    std::cout << "Введите количество элементов в одномерном массиве (не более " << MAX_SIZE << "): ";
    std::cin >> size;
    
    if (size == 0 || size > MAX_SIZE) 
    {
        std::cout << "Ошибка: некорректный размер массива!" << std::endl;
        return;
    }
    
    int array[MAX_SIZE];
    std::cout << "Введите " << size << " целых чисел: ";
    
    for (size_t i = 0; i < size; ++i) 
    {
        std::cin >> array[i];
    }
    
    bool result = check_product_one_dimensional(array, size);
    std::cout   << "Произведение элементов массива " 
                << (result ? "является" : "НЕ является") 
                << " трёхзначным числом." << std::endl;
}

void input_two_dimensional_array() 
{
    size_t rows, cols;
    std::cout << "Введите количество строк матрицы (не более " << MAX_ROWS << "): ";
    std::cin >> rows;
    std::cout << "Введите количество столбцов матрицы (не более " << MAX_COLS << "): ";
    std::cin >> cols;
    
    if (rows == 0 || rows > MAX_ROWS || cols == 0 || cols > MAX_COLS) 
    {
        std::cout << "Ошибка: некорректные размеры матрицы!" << std::endl;
        return;
    }
    
    int matrix[MAX_ROWS][MAX_COLS];
    std::cout << "Введите элементы матрицы " << rows << "x" << cols << ":" << std::endl;
    
    for (size_t i = 0; i < rows; ++i) 
    {
        for (size_t j = 0; j < cols; ++j) 
        {
            std::cout << "Элемент [" << i << "][" << j << "]: ";
            std::cin >> matrix[i][j];
        }
    }
    
    bool result = check_product_two_dimensional(matrix, rows, cols);
    std::cout   << "Произведение элементов матрицы " 
                << (result ? "является" : "НЕ является") 
                << " трёхзначным числом." << std::endl;
}

int main() 
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    int choice;
    
    std::cout << "=== Проверка произведения элементов на трёхзначность ===" << std::endl;
    std::cout << "Выберите тип массива:" << std::endl;
    std::cout << "1 - Одномерный массив" << std::endl;
    std::cout << "2 - Двумерный массив" << std::endl;
    std::cout << "Ваш выбор (1 или 2): ";
    std::cin >> choice;
    
    switch (choice) 
    {
        case 1:
            input_one_dimensional_array();
            break;
            
        case 2:
            input_two_dimensional_array();
            break;
            
        default:
            std::cout << "Ошибка: некорректный выбор!" << std::endl;
            return 1;
    }
    
    return 0;
}