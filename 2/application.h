#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>

class Application {
public:
    void run();

private:
    static const std::string APP_TITLE;
    static const std::string SEPARATOR;
    static const int DEFAULT_DOT_COUNT;
    static const int DEFAULT_DOT_DELAY_MS;
    static const int DEFAULT_SEPARATOR_LENGTH;
    static const char DEFAULT_SEPARATOR_SYMBOL;
    static const int ANIMATION_STEP_DELAY_MS;
    static const int EXPECTED_NUMBER_COUNT;
    static const int MAX_INPUT_ATTEMPTS;

    void showProgressDots(int count = DEFAULT_DOT_COUNT, int delay_ms = DEFAULT_DOT_DELAY_MS);
    void delay(int milliseconds);
    void printSeparator(char symbol = DEFAULT_SEPARATOR_SYMBOL, int length = DEFAULT_SEPARATOR_LENGTH);
    void printBoxedText(const std::string& text);
    
    int getIntegerInput(const std::string& prompt);
    std::vector<int> getNumbersFromLine(int expected_count = EXPECTED_NUMBER_COUNT);
    bool validateNumbersCount(const std::vector<int>& numbers, int expected_count);
    
    int findMin(int first_number, int second_number);
    int findMinOfFour(int a, int b, int c, int d);
    int findMinOfFour(const std::vector<int>& numbers);
    std::string getFormulaString(int a, int b, int c, int d);
    
    void displayHeader();
    void displaySimpleResult(const std::vector<int>& numbers, int result, const std::string& formula);
    void displayAnimatedResult(int a, int b, int c, int d, int result, int min1, int min2);
    void displayGraphicCalculation(const std::vector<int>& numbers);
    
    void runSimpleVersion();
    void runAnimatedVersion();
    void runGraphicVersion();
    void processMenuChoice(int choice);
};

#endif