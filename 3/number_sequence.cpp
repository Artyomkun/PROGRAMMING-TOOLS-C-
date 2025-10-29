#include "number_sequence.h"

void printSequenceWithFor() {
    cout << "Цикл for:" << endl;
    for (double currentValue = SEQUENCE_START; currentValue <= SEQUENCE_END; currentValue += SEQUENCE_STEP) {
        cout << currentValue << " " << currentValue + OFFSET_PLUS << " " << currentValue - OFFSET_MINUS << endl;
    }
    cout << endl;
}

void printSequenceWithWhile() {
    cout << "Цикл while:" << endl;
    double currentValue = SEQUENCE_START;
    while (currentValue <= SEQUENCE_END) {
        cout << currentValue << " " << currentValue + OFFSET_PLUS << " " << currentValue - OFFSET_MINUS << endl;
        currentValue += SEQUENCE_STEP;
    }
    cout << endl;
}

void printSequenceWithDoWhile() {
    cout << "Цикл do while:" << endl;
    double currentValue = SEQUENCE_START;
    do {
        cout << currentValue << " " << currentValue + OFFSET_PLUS << " " << currentValue - OFFSET_MINUS << endl;
        currentValue += SEQUENCE_STEP;
    } while (currentValue <= SEQUENCE_END);
    cout << endl;
}

int main() {
    cout << fixed << setprecision(1);
    
    printSequenceWithFor();
    printSequenceWithWhile();
    printSequenceWithDoWhile();
    
    return 0;
}