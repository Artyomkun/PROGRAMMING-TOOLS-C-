#include "visual_finder.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;

int VisualMinFinder::findMin(int first_number, int second_number) {
    return (first_number < second_number) ? first_number : second_number;
}

void VisualMinFinder::run() {
    const string HEADER_TITLE = "MINIMUM VALUE FINDER - VISUAL";
    const string HEADER_SEPARATOR = "=============================";
    const string PROCESS_TITLE = "CALCULATION PROCESS:";
    const string PROCESS_SEPARATOR = "====================";
    cout << HEADER_TITLE << endl;
    cout << HEADER_SEPARATOR << endl << endl;
    vector<int> numbers = getNumbersInput();
    cout << endl << PROCESS_TITLE << endl;
    cout << PROCESS_SEPARATOR << endl;
    delay(STEP_DELAY_MS);
    cout << endl << "STEP 1: Compare first two numbers" << endl;
    int min1 = findMin(numbers[0], numbers[1]);
    drawComparisonBox(numbers[0], numbers[1], min1, "First Pair");
    delay(RESULT_DELAY_MS);
    cout << endl << "STEP 2: Compare second two numbers" << endl;
    int min2 = findMin(numbers[2], numbers[3]);
    drawComparisonBox(numbers[2], numbers[3], min2, "Second Pair");
    delay(RESULT_DELAY_MS);
    cout << endl << "STEP 3: Compare results" << endl;
    int final_result = findMin(min1, min2);
    drawComparisonBox(min1, min2, final_result, "Final Comparison");
    delay(RESULT_DELAY_MS);
    cout << endl;
    drawFinalResult(final_result, numbers);
    cout << endl << "Numbers: ";
    for (size_t i = 0; i < numbers.size(); ++i) {
        cout << numbers[i];
        if (i < numbers.size() - 1) cout << ", ";
    }
    cout << endl << "Formula: min(min(" << numbers[0] << ", " << numbers[1] 
            << "), min(" << numbers[2] << ", " << numbers[3] << "))" << endl;
    cout << endl << "Press Enter to continue...";
    cin.get();
}

const int VisualMinFinder::BOX_WIDTH = 28;
const int VisualMinFinder::LABEL_WIDTH = 14;
const int VisualMinFinder::VALUE_WIDTH = 14;
const int VisualMinFinder::STEP_DELAY_MS = 500;
const int VisualMinFinder::RESULT_DELAY_MS = 1000;
const int VisualMinFinder::EXPECTED_NUMBERS_COUNT = 4;

string VisualMinFinder::formatLine(const string& label, int value) {
    stringstream ss;
    ss << left << setw(LABEL_WIDTH) << label << right << setw(VALUE_WIDTH) << value;
    return ss.str();
}

string VisualMinFinder::centerString(const string& text, int width) {
    int text_length = static_cast<int>(text.length());
    if (text_length >= width) return text;
    int left_padding = (width - text_length) / 2;
    int right_padding = width - text_length - left_padding;
    return string(left_padding, ' ') + text + string(right_padding, ' ');
}

void VisualMinFinder::delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void VisualMinFinder::drawComparisonBox(int number_a, int number_b, int result, const string& title) {
    const string horizontal_line = "+" + string(BOX_WIDTH, '-') + "+";
    const string arrow_line = "|" + centerString("V", BOX_WIDTH) + "|";
    cout << horizontal_line << endl;
    cout << "|" << centerString(title, BOX_WIDTH) << "|" << endl;
    cout << horizontal_line << endl;
    cout << "|" << formatLine("Number A:", number_a) << "|" << endl;
    cout << "|" << formatLine("Number B:", number_b) << "|" << endl;
    cout << arrow_line << endl;
    cout << "|" << formatLine("Minimum:", result) << "|" << endl;
    cout << horizontal_line << endl;
}

void VisualMinFinder::drawFinalResult(int result, const vector<int>& numbers) {
    const string double_line = "+" + string(BOX_WIDTH, '=') + "+";
    cout << double_line << endl;
    cout << "|" << centerString("FINAL RESULT", BOX_WIDTH) << "|" << endl;
    cout << "|" << centerString(to_string(result), BOX_WIDTH) << "|" << endl;
    cout << double_line << endl;
}

vector<int> VisualMinFinder::getNumbersInput() {
    vector<int> numbers(EXPECTED_NUMBERS_COUNT);
    const int MAX_ATTEMPTS = 3;
    for (int i = 0; i < EXPECTED_NUMBERS_COUNT; ++i) {
        int attempts = 0;
        bool valid_input = false;
        while (attempts < MAX_ATTEMPTS && !valid_input) {
            cout << "Number " << (i + 1) << ": ";
            if (cin >> numbers[i]) {
                valid_input = true;
            } else {
                cin.clear();
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                cout << "Invalid input! Please enter an integer." << endl;
                ++attempts;
            }
        }
        if (!valid_input) {
            cout << "Using default value 0." << endl;
            numbers[i] = 0;
        }
    }
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    return numbers;
}
