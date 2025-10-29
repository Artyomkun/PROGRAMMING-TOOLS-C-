#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <vector>
#include <string>

class InputUtils {
public:
    static int getIntegerInput(const std::string& prompt);
    static std::vector<int> getNumbersFromLine(int expected_count);
    static bool validateNumbersCount(const std::vector<int>& numbers, int expected_count);

private:
    static const int MAX_INPUT_ATTEMPTS;
    static const char NUMBER_DELIMITER;
    static const std::string DEFAULT_ERROR_MESSAGE;
};

#endif