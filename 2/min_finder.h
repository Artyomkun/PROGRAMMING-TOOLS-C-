#ifndef MIN_FINDER_H
#define MIN_FINDER_H

#include <vector>

class MinFinder {
public:
    int findMin(int a, int b);       
    int findMinOfFour(int a, int b, int c, int d);  
    
    void run();    

private:
    void showProgressDots(int count, int delay_ms);
    void delay(int ms);
    void printSeparator(char symbol, int length);
    int getIntegerInput(const std::string& prompt);
    std::vector<int> getNumbersFromLine(int expected_count);
    bool validateNumbersCount(const std::vector<int>& numbers, int expected_count);
    void runSimpleVersion();
    void runAnimatedVersion();
    void processMenuChoice(int choice);
    
    static const int DEFAULT_DOT_COUNT;
    static const int ANIMATION_STEP_DELAY_MS;
    static const int EXPECTED_NUMBER_COUNT;
};

#endif