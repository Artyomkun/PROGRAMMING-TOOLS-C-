#include <iostream>
#include <utility>
#include <clocale>

std::size_t calculate_string_length(const char* string_ptr) {
    const char* string_begin = string_ptr;
    
    while (*string_ptr != '\0') {
        ++string_ptr;
    }
    
    return string_ptr - string_begin;
}

std::size_t count_digits_in_string(const char* string_ptr) {
    std::size_t digit_count = 0;
    
    while (*string_ptr != '\0') {
        if (*string_ptr >= '0' && *string_ptr <= '9') {
            ++digit_count;
        }
        ++string_ptr;
    }
    
    return digit_count;
}

std::pair<std::size_t, std::size_t> analyze_string_separate(const char* string_ptr) {
    const std::size_t length = calculate_string_length(string_ptr);
    const std::size_t digit_count = count_digits_in_string(string_ptr);
    
    return std::make_pair(length, digit_count);
}

int main() {
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    const std::size_t MAX_BUFFER_SIZE = 256;
    char user_input[MAX_BUFFER_SIZE];
    
    std::cout << "Введите строку для анализа (максимум " 
                << MAX_BUFFER_SIZE - 1 << " символов):\n";
    std::cin.getline(user_input, MAX_BUFFER_SIZE);
    
    const std::pair<std::size_t, std::size_t> results = 
        analyze_string_separate(user_input);
    
    std::cout << "\nДлина строки: " << results.first << std::endl;
    std::cout << "Количество цифр: " << results.second << std::endl;
    
    return 0;
}