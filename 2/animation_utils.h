#ifndef ANIMATION_UTILS_H
#define ANIMATION_UTILS_H

#include <string>

class AnimationUtils {
public:
    static void showProgressDots(int count = DEFAULT_DOT_COUNT, 
                                int delay_ms = DEFAULT_DOT_DELAY_MS);
    static void delay(int milliseconds);
    static void printSeparator(char symbol = DEFAULT_SEPARATOR_SYMBOL, 
                                int length = DEFAULT_SEPARATOR_LENGTH);
    static void printBoxedText(const std::string& text);

private:
    static const int DEFAULT_DOT_COUNT;
    static const int DEFAULT_DOT_DELAY_MS;
    static const char DEFAULT_SEPARATOR_SYMBOL;
    static const int DEFAULT_SEPARATOR_LENGTH;
    static const int BOX_WIDTH;
};

#endif