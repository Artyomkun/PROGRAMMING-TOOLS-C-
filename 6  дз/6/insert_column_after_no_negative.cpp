#include <iostream>
#include <utility>

bool has_no_negative_elements(const int* column, size_t rows)
{
    for (size_t i = 0; i < rows; ++i)
    {
        if (column[i] < 0)
        {
            return false;
        }
    }
    return true;
}

void insert_column_after_no_negative(int**& matrix, size_t& rows, size_t& cols)
{
    if (rows == 0 || cols == 0 || matrix == nullptr)
    {
        return;
    }

    size_t target_col = cols;
    for (size_t j = 0; j < cols; ++j)
    {
        bool no_negative = true;
        for (size_t i = 0; i < rows; ++i)
        {
            if (matrix[i][j] < 0)
            {
                no_negative = false;
                break;
            }
        }
        
        if (no_negative)
        {
            target_col = j;
            break;
        }
    }

    if (target_col == cols)
    {
        std::cout << "Столбец без отрицательных элементов не найден." << std::endl;
        return;
    }

    size_t new_cols = cols + 1;
    int** new_matrix = new int*[rows];
    
    for (size_t i = 0; i < rows; ++i)
    {
        new_matrix[i] = new int[new_cols];
        
        for (size_t j = 0; j <= target_col; ++j)
        {
            new_matrix[i][j] = matrix[i][j];
        }
        
        new_matrix[i][target_col + 1] = 0;
        
        for (size_t j = target_col + 1; j < cols; ++j)
        {
            new_matrix[i][j + 1] = matrix[i][j];
        }
    }

    for (size_t i = 0; i < rows; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
    
    matrix = new_matrix;
    cols = new_cols;
}

void print_matrix(const int* const* matrix, size_t rows, size_t cols)
{
    std::cout << "Матрица " << rows << "x" << cols << ":" << std::endl;
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    size_t rows, cols;
    
    std::cout << "=== Вставка столбца после столбца без отрицательных элементов ===" << std::endl;
    std::cout << "Введите количество строк матрицы: ";
    std::cin >> rows;
    std::cout << "Введите количество столбцов матрицы: ";
    std::cin >> cols;
    
    if (rows == 0 || cols == 0)
    {
        std::cout << "Ошибка: размеры матрицы должны быть больше 0!" << std::endl;
        return 1;
    }
    
    int** matrix = new int*[rows];
    std::cout << "Введите элементы матрицы " << rows << "x" << cols << ":" << std::endl;
    
    for (size_t i = 0; i < rows; ++i)
    {
        matrix[i] = new int[cols];
        std::cout << "Строка " << i << ": ";
        for (size_t j = 0; j < cols; ++j)
        {
            std::cin >> matrix[i][j];
        }
    }
    
    std::cout << "\nИсходная матрица:" << std::endl;
    print_matrix(matrix, rows, cols);
    
    insert_column_after_no_negative(matrix, rows, cols);
    
    std::cout << "\nМатрица после вставки столбца:" << std::endl;
    print_matrix(matrix, rows, cols);
    
    for (size_t i = 0; i < rows; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
    
    return 0;
}