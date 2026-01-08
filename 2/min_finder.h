#ifndef MIN_FINDER_H
#define MIN_FINDER_H

#include <vector>
#include <string> 

class MinFinder {
public:
    int findMin(int a, int b);       
    int findMinOfFour(int a, int b, int c, int d);
    void showCalculationProcess(int a, int b, int c, int d);
    void displayResult(int a, int b, int c, int d, int result);
    static void showCalculationProcess(int a, int b, int result);
    int getIntegerInput(const std::string& prompt);
    void run();    

private:
    void showProgressDots(int count, int delay_ms);
    void delay(int ms);
    void printSeparator(char symbol, int length);
    std::vector<int> getNumbersFromLine(int expected_count);
    bool validateNumbersCount(const std::vector<int>& numbers, int expected_count);
    void runSimpleVersion();
    void runAnimatedVersion();
    void processMenuChoice(int choice);
    
    static const int ANIMATION_DELAY_MS;
    static const std::string PROGRAM_TITLE; 
    static const int DEFAULT_DOT_COUNT;
    static const int ANIMATION_STEP_DELAY_MS;
    static const int EXPECTED_NUMBER_COUNT;
};

#endif