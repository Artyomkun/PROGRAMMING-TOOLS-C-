#include "number_grid.h"

void printGridWithFor() {
    cout << "Цикл for:" << endl;
    for (int rowStart = GRID_START; rowStart <= GRID_END; rowStart += ROW_SIZE) {
        for (int number = rowStart; number < rowStart + VISIBLE_NUMBERS; ++number) {
            cout << number << "  ";
        }
        cout << "...    " << rowStart + ROW_SIZE - 1 << endl;
    }
    cout << endl;
}

void printGridWithWhile() {
    cout << "Цикл while:" << endl;
    int rowStart = GRID_START;
    while (rowStart <= GRID_END) {
        int number = rowStart;
        while (number < rowStart + VISIBLE_NUMBERS) {
            cout << number << "  ";
            ++number;
        }
        cout << "...    " << rowStart + ROW_SIZE - 1 << endl;
        rowStart += ROW_SIZE;
    }
    cout << endl;
}

void printGridWithDoWhile() {
    cout << "Цикл do while:" << endl;
    int rowStart = GRID_START;
    do {
        int number = rowStart;
        do {
            cout << number << "  ";
            ++number;
        } while (number < rowStart + VISIBLE_NUMBERS);
        cout << "...    " << rowStart + ROW_SIZE - 1 << endl;
        rowStart += ROW_SIZE;
    } while (rowStart <= GRID_END);
    cout << endl;
}

int main() {
    printGridWithFor();
    printGridWithWhile();
    printGridWithDoWhile();
    
    return 0;
}