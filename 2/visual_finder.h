#ifndef VISUAL_FINDER_H
#define VISUAL_FINDER_H

#include <string>
#include <vector>

class VisualMinFinder {
public:
    static int findMin(int first_number, int second_number);
    void run();
    void execute();

private:
    static void delay(int milliseconds);
    static std::string formatLine(const std::string& label, int value);
    static std::string centerString(const std::string& text, int width);
    static void drawComparisonBox(int number_a, int number_b, int result, const std::string& title);
    static void drawFinalResult(int result, const std::vector<int>& numbers);
    static std::vector<int> getNumbersInput();
    static const int BOX_WIDTH;
    static const int LABEL_WIDTH;
    static const int VALUE_WIDTH;
    static const int STEP_DELAY_MS;
    static const int RESULT_DELAY_MS;
    static const int EXPECTED_NUMBERS_COUNT;
};

#endif