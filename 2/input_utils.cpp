#include "input_utils.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using std::numeric_limits;
using std::streamsize;

// ОСНОВНЫЕ ФУНКЦИИ ПО ЗАДАНИЮ - НАВЕРХУ

int InputUtils::getIntegerInput(const string& prompt) {
    int value;
    int attempts = 0;
    
    while (attempts < MAX_INPUT_ATTEMPTS) {
        cout << prompt;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << DEFAULT_ERROR_MESSAGE;
            ++attempts;
        } else {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            return value;
        }
    }
    
    cout << "Too many failed attempts. Using default value 0." << endl;
    return 0;
}

vector<int> InputUtils::getNumbersFromLine(int expected_count) {
    vector<int> numbers;
    string input;
    
    cout << "Enter " << expected_count << " numbers separated by spaces: ";
    getline(cin, input);
    
    stringstream ss(input);
    int number;
    
    while (ss >> number) {
        numbers.push_back(number);
    }
    
    if (numbers.size() != expected_count) {
        cout << "Warning: Expected " << expected_count 
             << " numbers, but got " << numbers.size() << endl;
    }
    
    return numbers;
}

const int InputUtils::MAX_INPUT_ATTEMPTS = 5;
const char InputUtils::NUMBER_DELIMITER = ' ';
const string InputUtils::DEFAULT_ERROR_MESSAGE = "Error! Please enter an integer: ";

bool InputUtils::validateNumbersCount(const vector<int>& numbers, int expected_count) {
    const bool is_valid = numbers.size() == expected_count;
    
    if (!is_valid) {
        cout << "Validation failed: Expected " << expected_count 
             << " numbers, but got " << numbers.size() << endl;
    }
    
    return is_valid;
}
