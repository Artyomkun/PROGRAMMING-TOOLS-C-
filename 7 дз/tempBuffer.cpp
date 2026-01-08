#include <iostream>
#include <cstring>
#include <windows.h>

const size_t MAX_STRING_LENGTH = 100;

char* double_character_in_string(const char* source_string, char character_to_double) {
    if (source_string == nullptr) {
        return nullptr;
    }

    size_t source_length = std::strlen(source_string);
    size_t character_count = 0;

    for (size_t i = 0; i < source_length; ++i) {
        if (source_string[i] == character_to_double) {
            ++character_count;
        }
    }

    char* result_string = new char[source_length + character_count + 1];
    size_t result_position = 0;

    for (size_t i = 0; i < source_length; ++i) {
        result_string[result_position] = source_string[i];
        ++result_position;

        if (source_string[i] == character_to_double) {
            result_string[result_position] = character_to_double;
            ++result_position;
        }
    }

    result_string[result_position] = '\0';
    return result_string;
}

int main() {
    char input_string[MAX_STRING_LENGTH];
    char character;

    std::cout << "==============================" << std::endl;
    std::cout << "DOUBLE A CHARACTER IN A STRING" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << std::endl;

    std::cout << "Enter a string: ";
    std::cin.getline(input_string, MAX_STRING_LENGTH);

    if (std::strlen(input_string) == 0) {
        std::cout << "Error: The string is empty!" << std::endl;
        return 1;
    }

    std::cout << "Enter a character to double: ";
    std::cin >> character;
    std::cin.ignore();

    char* result_string = double_character_in_string(input_string, character);

    if (result_string != nullptr) {
        std::cout << std::endl;
        std::cout << "==============================" << std::endl;
        std::cout << "            RESULT" << std::endl;
        std::cout << "==============================" << std::endl;
        std::cout << "Source string:        \"" << input_string << "\"" << std::endl;
        std::cout << "Character to double:  '" << character << "'" << std::endl;
        std::cout << "Result:               \"" << result_string << "\"" << std::endl;
        std::cout << "==============================" << std::endl;

        delete[] result_string;
    } else {
        std::cout << "Error: Could not process the string!" << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}