#include "animation_utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

const int AnimationUtils::DEFAULT_DOT_COUNT = 3;
const int AnimationUtils::DEFAULT_DOT_DELAY_MS = 300;
const char AnimationUtils::DEFAULT_SEPARATOR_SYMBOL = '=';
const int AnimationUtils::DEFAULT_SEPARATOR_LENGTH = 40;
const int AnimationUtils::BOX_WIDTH = 38;

void AnimationUtils::showProgressDots(int count, int delay_ms) {
    for (int i = 0; i < count; ++i) {
        cout << ".";
        cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    cout << endl;
}

void AnimationUtils::delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void AnimationUtils::printSeparator(char symbol, int length) {
    cout << string(length, symbol) << endl;
}

void AnimationUtils::printBoxedText(const string& text) {
    const int text_length = static_cast<int>(text.length());
    const int total_padding = BOX_WIDTH - text_length;
    const int left_padding = total_padding / 2;
    const int right_padding = total_padding - left_padding;
    
    const string TOP_LEFT = "+";
    const string TOP_RIGHT = "+";
    const string BOTTOM_LEFT = "+"; 
    const string BOTTOM_RIGHT = "+";
    const string VERTICAL = "|";
    const string HORIZONTAL = "-";
    
    cout << TOP_LEFT << string(BOX_WIDTH, '-') << TOP_RIGHT << endl;
    
    cout << VERTICAL;
    cout << string(left_padding, ' ');
    cout << text;
    cout << string(right_padding, ' ');
    cout << VERTICAL << endl;
    
    cout << BOTTOM_LEFT << string(BOX_WIDTH, '-') << BOTTOM_RIGHT << endl;
}
