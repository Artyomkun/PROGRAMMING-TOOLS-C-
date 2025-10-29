#include "digit_sum_checker.h"
#include <iostream>
#include <string>

#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif

DigitSumChecker::DigitSumChecker() 
    : m_number(0)
    , m_digitSum(0)
    , m_isMultipleOfSeven(false) {
}

void DigitSumChecker::InitializeConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void DigitSumChecker::DisplayWelcomeMessage() {
    std::cout << "==============================================" << std::endl;
    std::cout << "     TWO-DIGIT NUMBER DIGIT SUM CHECKER" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Checks if sum of digits is multiple of 7" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << std::endl;
}

void DigitSumChecker::GetUserInput() {
    std::cout << "Please enter a two-digit number (10-99): ";
    std::cin >> m_number;
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

bool DigitSumChecker::ValidateInput() const {
    if (std::cin.fail()) {
        std::cout << "ERROR: Invalid input. Please enter a numeric value." << std::endl;
        return false;
    }
    
    if (m_number < 10 || m_number > 99) {
        std::cout << "ERROR: Please enter a two-digit number (10-99)." << std::endl;
        return false;
    }
    
    return true;
}

void DigitSumChecker::CalculateDigitSum() {
    int tensDigit = m_number / 10;
    int unitsDigit = m_number % 10;
    m_digitSum = tensDigit + unitsDigit;
}

void DigitSumChecker::CheckMultipleOfSeven() {
    m_isMultipleOfSeven = (m_digitSum % 7 == 0);
}

void DigitSumChecker::DisplayResults() const {
    int tensDigit = m_number / 10;
    int unitsDigit = m_number % 10;
    
    std::cout << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "               CHECK RESULTS" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Input number: " << m_number << std::endl;
    std::cout << "Digits: " << tensDigit << " + " << unitsDigit << std::endl;
    std::cout << "Sum of digits: " << m_digitSum << std::endl;
    std::cout << std::endl;
    
    if (m_isMultipleOfSeven) {
        std::cout << "✓ The sum IS divisible by 7" << std::endl;
        std::cout << "  " << m_digitSum << " ÷ 7 = " << (m_digitSum / 7) << std::endl;
    } else {
        std::cout << "✗ The sum is NOT divisible by 7" << std::endl;
        std::cout << "  Remainder: " << (m_digitSum % 7) << std::endl;
    }
    
    std::cout << "==============================================" << std::endl;
}

bool DigitSumChecker::AskForAnotherCalculation() {
    char choice;
    
    while (true) {
        std::cout << std::endl;
        std::cout << "Do you want to check another number? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(10000, '\n');
        
        if (choice == 'y' || choice == 'Y') {
            return true;
        }
        else if (choice == 'n' || choice == 'N') {
            return false;
        }
        else {
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << std::endl;
        }
    }
}

void DigitSumChecker::Run() {
    InitializeConsole();
    
    bool continueChecking = true;
    
    while (continueChecking) {
        DisplayWelcomeMessage();
        GetUserInput();
        
        if (!ValidateInput()) {
            continue;
        }
        
        CalculateDigitSum();
        CheckMultipleOfSeven();
        DisplayResults();
        
        continueChecking = AskForAnotherCalculation();
    }
    
    std::cout << std::endl;
    std::cout << "Thank you for using Digit Sum Checker!" << std::endl;
}