#include "number_sequence.h"
#include <iostream>
#include <iomanip>
#include <locale>
using namespace std;
using namespace NumberSequence;

void NumberSequence::printSequenceWithFor() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    cout << "Цикл for:" << endl;
    for (double currentValue = SEQUENCE_START; currentValue <= SEQUENCE_END; currentValue += SEQUENCE_STEP) {
        cout << currentValue << " " << currentValue + OFFSET_PLUS << " " << currentValue - OFFSET_MINUS << endl;
    }
    cout << endl;
}

void NumberSequence::printSequenceWithWhile() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    cout << "Цикл while:" << endl;
    double currentValue = SEQUENCE_START;
    while (currentValue <= SEQUENCE_END) {
        cout << currentValue << " " << currentValue + OFFSET_PLUS << " " << currentValue - OFFSET_MINUS << endl;
        currentValue += SEQUENCE_STEP;
    }
    cout << endl;
}

void NumberSequence::printSequenceWithDoWhile() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    cout << "Цикл do while:" << endl;
    double currentValue = SEQUENCE_START;
    do {
        cout << currentValue << " " << currentValue + OFFSET_PLUS << " " << currentValue - OFFSET_MINUS << endl;
        currentValue += SEQUENCE_STEP;
    } while (currentValue <= SEQUENCE_END);
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    cout << fixed << setprecision(1);
    
    printSequenceWithFor();
    printSequenceWithWhile();
    printSequenceWithDoWhile();
    
    return 0;
}