#include <iostream>
#include <utility>

void remove_max_elements(int*& array, size_t& size)
{
    if (size == 0 || array == nullptr) 
    {
        return;
    }
    
    int max_element = array[0];
    for (size_t i = 1; i < size; ++i) 
    {
        if (array[i] > max_element) 
        {
            max_element = array[i];
        }
    }
    
    size_t new_size = 0;
    for (size_t i = 0; i < size; ++i) 
    {
        if (array[i] != max_element) 
        {
            ++new_size;
        }
    }
    
    if (new_size == 0) 
    {
        delete[] array;
        array = nullptr;
        size = 0;
        return;
    }
    
    int* new_array = new int[new_size];
    size_t new_index = 0;
    
    for (size_t i = 0; i < size; ++i) 
    {
        if (array[i] != max_element) 
        {
            new_array[new_index] = array[i];
            ++new_index;
        }
    }
    
    delete[] array;
    array = new_array;
    size = new_size;
}

void print_array(const int* array, size_t size) 
{
    std::cout << "[ ";
    for (size_t i = 0; i < size; ++i) 
    {
        std::cout << array[i];
        if (i < size - 1) 
        {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;
}

int main() 
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    size_t size;
    
    std::cout << "=== Удаление всех максимальных элементов из массива ===" << std::endl;
    std::cout << "Введите размер массива: ";
    std::cin >> size;
    
    if (size == 0) 
    {
        std::cout << "Ошибка: размер массива должен быть больше 0!" << std::endl;
        return 1;
    }
    
    int* array = new int[size];
    
    std::cout << "Введите " << size << " целых чисел: ";
    for (size_t i = 0; i < size; ++i) 
    {
        std::cin >> array[i];
    }
    
    std::cout << "\nИсходный массив: ";
    print_array(array, size);
    
    remove_max_elements(array, size);
    
    std::cout << "Массив после удаления максимальных элементов: ";
    if (size > 0) 
    {
        print_array(array, size);
    }
    else 
    {
        std::cout << "массив пуст" << std::endl;
    }
    
    if (array != nullptr) 
    {
        delete[] array;
    }
    
    return 0;
}