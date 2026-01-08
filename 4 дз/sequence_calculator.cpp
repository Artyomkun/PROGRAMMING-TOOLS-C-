#include "sequence_calculator.h"
#include <iostream>
#include <iomanip>
#include <cmath>

SequenceCalculator::SequenceCalculator(long double firstTerm, long double secondTerm) 
    : FIRST_TERM(firstTerm), SECOND_TERM(secondTerm) {
}

long double SequenceCalculator::calculateTerm(int termNumber) const {
    if (termNumber <= 0) {
        return 0.0L;
    }
    if (termNumber == 1) {
        return FIRST_TERM;
    }
    if (termNumber == 2) {
        return SECOND_TERM;
    }
    
    long double previousTerm = SECOND_TERM;
    long double termBeforePrevious = FIRST_TERM;
    long double currentTerm = 0.0L;
    
    for (int i = 3; i <= termNumber; ++i) {
        currentTerm = termBeforePrevious / 4.0L + 5.0L / (previousTerm * previousTerm);
        termBeforePrevious = previousTerm;
        previousTerm = currentTerm;
    }
    
    return currentTerm;
}

void SequenceCalculator::displaySequenceInfo() const {
    std::cout << "==============================================" << std::endl;
    std::cout << "          КАЛЬКУЛЯТОР ПОСЛЕДОВАТЕЛЬНОСТИ     " << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Формула: b1 = " << FIRST_TERM << ", b2 = " << SECOND_TERM << std::endl;
    std::cout << "        bn = bn-2/4 + 5/bn-1^2" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << std::endl;
}

void SequenceCalculator::printSequenceUpTo(int maxTerm) const {
    if (maxTerm <= 0) {
        std::cout << "Ошибка: максимальный член должен быть положительным." << std::endl;
        return;
    }
    
    std::cout << "Первые " << maxTerm << " членов последовательности:" << std::endl;
    
    for (int i = 1; i <= maxTerm; ++i) {
        long double term = calculateTerm(i);
        std::cout << "b" << i << " = ";
        
        if (term == 0.0L) {
            std::cout << "0.0";
        } else if (std::abs(term) < 1e-10L) {
            std::cout << std::scientific << std::setprecision(15) << term;
        } else if (std::abs(term) > 1e10L) {
            std::cout << std::scientific << std::setprecision(6) << term;
        } else if (std::abs(term) < 0.001L) {
            std::cout << std::fixed << std::setprecision(10) << term;
        } else {
            std::cout << std::fixed << std::setprecision(6) << term;
        }
        std::cout << std::endl;
    }
    std::cout << std::defaultfloat;
    std::cout << std::endl;
}