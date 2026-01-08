#include <windows.h>
#include <locale.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include "sequence_calculator.h"

void displayMenu() {
    std::cout << "==============================================" << std::endl;
    std::cout << "                   МЕНЮ                      " << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "1. Вычислить конкретный член последовательности" << std::endl;
    std::cout << "2. Вывести несколько первых членов" << std::endl;
    std::cout << "3. Показать информацию о последовательности" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Выберите опцию: ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

int getValidatedInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail() || value < 0) {
            std::cout << "Ошибка: введите положительное целое число." << std::endl;
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

void calculateSpecificTerm(const SequenceCalculator& calculator) {
    int termNumber = getValidatedInput("Введите номер члена последовательности: ");
    
    if (termNumber <= 0) {
        std::cout << "Ошибка: номер должен быть положительным." << std::endl;
        return;
    }
    
    long double result = calculator.calculateTerm(termNumber);
    
    std::cout << "b" << termNumber << " = ";
    
    if (result == 0.0L) {
        std::cout << "0.0";
    } else if (std::abs(result) < 1e-10L) {
        std::cout << std::scientific << std::setprecision(15) << result;
    } else if (std::abs(result) > 1e10L) {
        std::cout << std::scientific << std::setprecision(6) << result;
    } else if (std::abs(result) < 0.001L) {
        std::cout << std::fixed << std::setprecision(10) << result;
    } else {
        std::cout << std::fixed << std::setprecision(6) << result;
    }
    std::cout << std::endl << std::endl;
    std::cout << std::defaultfloat;
}

void displayMultipleTerms(const SequenceCalculator& calculator) {
    int maxTerm = getValidatedInput("Сколько членов последовательности вывести? ");
    
    if (maxTerm <= 0) {
        std::cout << "Ошибка: количество должно быть положительным." << std::endl;
        return;
    }
    
    calculator.printSequenceUpTo(maxTerm);
}

int main() {
    setlocale(LC_ALL, "Russian");
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    
    const long double DEFAULT_FIRST_TERM = 1.0L;
    const long double DEFAULT_SECOND_TERM = 2.0L;
    
    SequenceCalculator calculator(DEFAULT_FIRST_TERM, DEFAULT_SECOND_TERM);
    
    std::cout << "Добро пожаловать в калькулятор последовательностей!" << std::endl;
    std::cout << std::endl;
    
    calculator.displaySequenceInfo();
    
    int choice = 0;
    
    do {
        displayMenu();
        std::cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                calculateSpecificTerm(calculator);
                break;
                
            case 2:
                displayMultipleTerms(calculator);
                break;
                
            case 3:
                calculator.displaySequenceInfo();
                break;
                
            case 0:
                std::cout << "До свидания!" << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                std::cout << std::endl;
                break;
        }
        
    } while (choice != 0);
    
    return 0;
}