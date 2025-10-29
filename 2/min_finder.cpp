#include "min_finder.h"
#include "animation_utils.h"
#include "input_utils.h"
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;

const int MinFinder::ANIMATION_DELAY_MS = 500;
const char* const MinFinder::PROGRAM_TITLE = "MINIMUM VALUE FINDER";

int MinFinder::findMin(int a, int b) {
    return a < b ? a : b;
}

int MinFinder::findMinOfFour(int a, int b, int c, int d) {
    return std::min(std::min(a, b), std::min(c, d));
}

void MinFinder::showCalculationProcess(int a, int b, int c, int d) {
    cout << "\n";
    AnimationUtils::printSeparator();
    cout << "CALCULATION PROCESS" << endl;
    AnimationUtils::printSeparator();
    
    const string STEP_1_TEXT = "Step 1: Find min of first two numbers";
    const string STEP_2_TEXT = "Step 2: Find min of second two numbers"; 
    const string STEP_3_TEXT = "Step 3: Find overall minimum";
    
    cout << "\n" << STEP_1_TEXT << endl;
    cout << "Calculating min(" << a << ", " << b << ")";
    AnimationUtils::showProgressDots();
    int min1 = std::min(a, b);  
    cout << "Result: " << min1 << " ✓" << endl;
    AnimationUtils::delay(ANIMATION_DELAY_MS);
    
    cout << "\n" << STEP_2_TEXT << endl;
    cout << "Calculating min(" << c << ", " << d << ")";
    AnimationUtils::showProgressDots();
    int min2 = std::min(c, d);
    cout << "Result: " << min2 << " ✓" << endl;
    AnimationUtils::delay(ANIMATION_DELAY_MS);
    
    cout << "\n" << STEP_3_TEXT << endl;
    cout << "Calculating min(" << min1 << ", " << min2 << ")";
    AnimationUtils::showProgressDots();
    int result = std::min(min1, min2);
    cout << "Result: " << result << " ✓" << endl;
}

void MinFinder::displayResult(int a, int b, int c, int d, int result) {
    cout << "\n";
    cout << "FINAL RESULT: " << result << endl;
    
    cout << "\nNumbers: " << a << ", " << b << ", " << c << ", " << d << endl;
    cout << "Formula: min(min(" << a << ", " << b << "), min(" << c << ", " << d << "))" << endl;
}

void MinFinder::run() {
    cout << PROGRAM_TITLE << endl;
    AnimationUtils::printSeparator();
    cout << "Find minimum value from four numbers using min(a,b) function" << endl << endl;
    
    const int NUM_COUNT = 4;
    int numbers[NUM_COUNT];
    
    numbers[0] = InputUtils::getIntegerInput("Enter first number: ");
    numbers[1] = InputUtils::getIntegerInput("Enter second number: ");
    numbers[2] = InputUtils::getIntegerInput("Enter third number: ");
    numbers[3] = InputUtils::getIntegerInput("Enter fourth number: ");
    
    showCalculationProcess(numbers[0], numbers[1], numbers[2], numbers[3]);

    int result = findMinOfFour(numbers[0], numbers[1], numbers[2], numbers[3]);
    displayResult(numbers[0], numbers[1], numbers[2], numbers[3], result);
    
    cout << "\nPress Enter to exit...";
    std::cin.get();
}