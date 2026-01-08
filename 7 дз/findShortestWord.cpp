#include <iostream>
#include <string>
#include <cctype>
#include <utility>
#include <limits>

const size_t MAX_MESSAGE_LENGTH = 1000;

bool is_word_character(char character) {
    unsigned char c = static_cast<unsigned char>(character);
    return std::isalpha(c) || std::isdigit(c) || c >= 128;
}

std::pair<std::string, size_t> find_shortest_word(std::string& message) {
    std::string shortest_word;
    size_t shortest_length = std::numeric_limits<size_t>::max();
    size_t shortest_start = 0;

    size_t i = 0;
    const size_t message_len = message.length();

    while (i < message_len) {
        while (i < message_len && !is_word_character(message[i])) {
            ++i;
        }

        if (i >= message_len) {
            break;
        }

        const size_t word_start = i;
        
        while (i < message_len && is_word_character(message[i])) {
            ++i;
        }

        const size_t word_length = i - word_start;
        
        if (word_length < shortest_length) {
            shortest_word = message.substr(word_start, word_length);
            shortest_length = word_length;
            shortest_start = word_start;
        }
    }

    if (shortest_length != std::numeric_limits<size_t>::max()) {
        message.erase(shortest_start, shortest_length);
    }

    return std::make_pair(shortest_word, shortest_length);
}

void show_result(const std::string& shortest_word, size_t shortest_len, const std::string& new_message) {
    std::cout << "\n=================================\n";
    std::cout << "            RESULT\n";
    std::cout << "=================================\n";
    
    if (shortest_word.empty()) {
        std::cout << "No words found in the message.\n";
    } else {
        std::cout << "Shortest word: \"" << shortest_word << "\"\n";
        std::cout << "Length: " << shortest_len << " character(s)\n";
        std::cout << "\nMessage after removing shortest word:\n";
        std::cout << "\"" << new_message << "\"\n";
    }
    
    std::cout << "=================================\n";
}

int main() {
    std::string user_input;
    
    std::cout << "=================================\n";
    std::cout << "     FIND SHORTEST WORD\n";
    std::cout << "=================================\n\n";
    
    std::cout << "Enter message: ";
    std::getline(std::cin, user_input);
    
    std::string message_copy = user_input;
    std::pair<std::string, size_t> result = find_shortest_word(message_copy);
    
    show_result(result.first, result.second, message_copy);
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    
    return 0;
}