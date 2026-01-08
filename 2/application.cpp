#include "application.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

int Application::findMin(int first_number, int second_number) {
    return (first_number < second_number) ? first_number : second_number;
}

int Application::findMinOfFour(int a, int b, int c, int d) {
    return findMin(findMin(a, b), findMin(c, d));
}

int Application::findMinOfFour(const vector<int>& numbers) {
    if (numbers.size() != EXPECTED_NUMBER_COUNT) {
        throw invalid_argument("Exactly 4 numbers required");
    }
    return findMin(findMin(numbers[0], numbers[1]), findMin(numbers[2], numbers[3]));
}

void Application::run() {
    int choice;
    
    do {
        displayHeader();
        cout << "SELECT WORKING MODE:" << endl;
        cout << "1. Simple version" << endl;
        cout << "2. Animated version" << endl;
        cout << "3. Graphic version" << endl;
        cout << "4. Exit" << endl;
        
        string input;
        cout << "Your choice (1-4): ";
        getline(cin, input);
        
        stringstream ss(input);
        if (!(ss >> choice)) {
            choice = -1;
        }
        
        processMenuChoice(choice);
        
    } while (choice != 4);
}

const string Application::APP_TITLE = " MINIMUM VALUE FINDER";
const string Application::SEPARATOR = "==========================================";
const int Application::DEFAULT_DOT_COUNT = 3;
const int Application::DEFAULT_DOT_DELAY_MS = 300;
const int Application::DEFAULT_SEPARATOR_LENGTH = 40;
const char Application::DEFAULT_SEPARATOR_SYMBOL = '=';
const int Application::ANIMATION_STEP_DELAY_MS = 500;
const int Application::EXPECTED_NUMBER_COUNT = 4;
const int Application::MAX_INPUT_ATTEMPTS = 3;

void Application::showProgressDots(int count, int delay_ms) {
    for (int i = 0; i < count; ++i) {
        cout << ".";
        cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    cout << endl;
}

void Application::delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Application::printSeparator(char symbol, int length) {
    cout << string(length, symbol) << endl;
}

void Application::printBoxedText(const string& text) {
    const int BOX_WIDTH = 38;
    const string TOP_BORDER = "+" + string(BOX_WIDTH, '-') + "+";
    const string BOTTOM_BORDER = "+" + string(BOX_WIDTH, '-') + "+";
    
    cout << TOP_BORDER << endl;
    cout << "| " << text << string(BOX_WIDTH - text.length() - 2, ' ') << " |" << endl;
    cout << BOTTOM_BORDER << endl;
}

int Application::getIntegerInput(const string& prompt) {
    int value;
    int attempt_count = 0;
    
    while (attempt_count < MAX_INPUT_ATTEMPTS) {
        cout << prompt;
        
        string input_line;
        getline(cin, input_line);
        
        stringstream ss(input_line);
        
        if (ss >> value && ss.eof()) {
            return value;
        } else {
            ++attempt_count;
            if (attempt_count < MAX_INPUT_ATTEMPTS) {
                cout << "Error! Please enter an integer: ";
            }
        }
    }
    
    cout << "Too many failed attempts. Using default value 0." << endl;
    return 0;
}

vector<int> Application::getNumbersFromLine(int expected_count) {
    vector<int> numbers;
    string input;
    
    cout << "Enter " << expected_count << " numbers separated by spaces: ";
    getline(cin, input);
    
    stringstream ss(input);
    int number;
    
    while (ss >> number) {
        numbers.push_back(number);
    }
    
    return numbers;
}

bool Application::validateNumbersCount(const vector<int>& numbers, int expected_count) {
    const bool is_valid = numbers.size() == expected_count;
    
    if (!is_valid) {
        cout << "Error! Expected " << expected_count 
             << " numbers, but got " << numbers.size() << endl;
    }
    
    return is_valid;
}

string Application::getFormulaString(int a, int b, int c, int d) {
    return "min(min(" + to_string(a) + ", " + to_string(b) + 
        "), min(" + to_string(c) + ", " + to_string(d) + "))";
}

void Application::displayHeader() {
    cout << "\n" << SEPARATOR << endl;
    cout << APP_TITLE << endl;
    cout << "        from four numbers" << endl;
    cout << SEPARATOR << endl;
}

void Application::displaySimpleResult(const vector<int>& numbers, int result, const string& formula) {
    cout << "\n" << SEPARATOR << endl;
    cout << "RESULT:" << endl;
    cout << "Numbers: " << numbers[0] << ", " << numbers[1] << ", " 
        << numbers[2] << ", " << numbers[3] << endl;
    cout << "Formula: " << formula << endl;
    cout << "Minimum value: " << result << endl;
    cout << SEPARATOR << endl;
}

void Application::displayAnimatedResult(int a, int b, int c, int d, int result, int min1, int min2) {
    string star_separator = SEPARATOR;
    
    cout << "\n" << star_separator << endl;
    cout << "FINAL RESULT: " << result << endl;
    cout << star_separator << endl;
    
    cout << "\n CALCULATION DETAILS:" << endl;
    cout << "Original numbers: " << a << ", " << b << ", " << c << ", " << d << endl;
    cout << "min(" << a << ", " << b << ") = " << min1 << endl;
    cout << "min(" << c << ", " << d << ") = " << min2 << endl;
    cout << "min(" << min1 << ", " << min2 << ") = " << result << endl;
}

void Application::displayGraphicCalculation(const vector<int>& numbers) {
    cout << "\n";
    printBoxedText("CALCULATION VISUALIZATION");
    
    cout << "\n+---------- STEP 1 ----------+" << endl;
    cout << "| First pair: " << numbers[0] << " and " << numbers[1] << " |" << endl;
    delay(ANIMATION_STEP_DELAY_MS);
    int min1 = findMin(numbers[0], numbers[1]);
    cout << "| Result: " << min1 << "           |" << endl;
    cout << "+---------------------------+" << endl;
    
    cout << "\n+---------- STEP 2 ----------+" << endl;
    cout << "| Second pair: " << numbers[2] << " and " << numbers[3] << " |" << endl;
    delay(ANIMATION_STEP_DELAY_MS);
    int min2 = findMin(numbers[2], numbers[3]);
    cout << "| Result: " << min2 << "           |" << endl;
    cout << "+---------------------------+" << endl;
    
    cout << "\n+--------- FINAL STEP ---------+" << endl;
    cout << "| Compare: " << min1 << " and " << min2 << "   |" << endl;
    delay(ANIMATION_STEP_DELAY_MS);
    int result = findMin(min1, min2);
    cout << "| MINIMUM: " << result << "            |" << endl;
    cout << "+--------------------------+" << endl;
    
    cout << "\n==============================" << endl;
    cout << "  RESULT: Minimum = " << result << endl;
    cout << "==============================" << endl;
}

void Application::runSimpleVersion() {
    cout << "\n*** SIMPLE VERSION ***" << endl;
    printSeparator();
    
    vector<int> numbers = getNumbersFromLine(EXPECTED_NUMBER_COUNT);
    
    if (!validateNumbersCount(numbers, EXPECTED_NUMBER_COUNT)) {
        return;
    }
    
    int result = findMinOfFour(numbers);
    string formula = getFormulaString(numbers[0], numbers[1], numbers[2], numbers[3]);
    
    displaySimpleResult(numbers, result, formula);
}

void Application::runAnimatedVersion() {
    cout << "\n*** ANIMATED VERSION ***" << endl;
    printSeparator();
    
    int a = getIntegerInput("Enter first number: ");
    int b = getIntegerInput("Enter second number: ");
    int c = getIntegerInput("Enter third number: ");
    int d = getIntegerInput("Enter fourth number: ");
    
    cout << "\n" << SEPARATOR << endl;
    cout << "CALCULATION PROCESS" << endl;
    cout << SEPARATOR << endl;
    
    cout << "\nStep 1: Find minimum of first two numbers" << endl;
    cout << "    Calculating min(" << a << ", " << b << ")";
    showProgressDots();
    int min1 = findMin(a, b);
    cout << "    Result: " << min1 << " ✓" << endl;
    delay(ANIMATION_STEP_DELAY_MS);
    
    cout << "\nStep 2: Find minimum of second two numbers" << endl;
    cout << "    Calculating min(" << c << ", " << d << ")";
    showProgressDots();
    int min2 = findMin(c, d);
    cout << "    Result: " << min2 << " ✓" << endl;
    delay(ANIMATION_STEP_DELAY_MS);
    
    cout << "\nStep 3: Find overall minimum" << endl;
    cout << "    Calculating min(" << min1 << ", " << min2 << ")";
    showProgressDots();
    int final_result = findMin(min1, min2);
    cout << "    Result: " << final_result << " ✓" << endl;
    
    displayAnimatedResult(a, b, c, d, final_result, min1, min2);
}

void Application::runGraphicVersion() {
    cout << "\n*** GRAPHIC VERSION ***" << endl;
    printSeparator();
    
    vector<int> numbers = getNumbersFromLine(EXPECTED_NUMBER_COUNT);
    
    if (!validateNumbersCount(numbers, EXPECTED_NUMBER_COUNT)) {
        return;
    }
    
    displayGraphicCalculation(numbers);
}

void Application::processMenuChoice(int choice) {
    switch (choice) {
        case 1:
            runSimpleVersion();
            break;
        case 2:
            runAnimatedVersion();
            break;
        case 3:
            runGraphicVersion();
            break;
        case 4:
            cout << "Exiting program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
    }
    
    if (choice != 4) {
        cout << "\nPress Enter to continue...";
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    }
}
