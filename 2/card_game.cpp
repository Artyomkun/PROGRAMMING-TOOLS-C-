#include "card_game.h"

void initializeConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

string getCardName(int cardValue) {
    switch (cardValue) {
        case ACE_VALUE: return "Туз";
        case KING_VALUE: return "Король";
        case QUEEN_VALUE: return "Дама";
        case JACK_VALUE: return "Валет";
        case TEN_VALUE: return "Десятка";
        default: return std::to_string(cardValue);
    }
}

bool isValidCard(int cardValue) {
    return cardValue >= MIN_CARD_VALUE && cardValue <= MAX_CARD_VALUE;
}

void printGameRules() {
    cout << "=== Карточная игра ===" << endl;
    cout << "Правила достоинств карт:" << endl;
    cout << "Туз - 14, Король - 13, Дама - 12, Валет - 11, Десятка - 10" << endl;
    cout << "Диапазон используемых карт: от " << MIN_CARD_VALUE << " до " << MAX_CARD_VALUE << endl;
}

void printAllCards() {
    cout << "Все карты в допустимом диапазоне:" << endl;
    for (int i = MIN_CARD_VALUE; i <= MAX_CARD_VALUE; ++i) {
        string currentCardName = getCardName(i);
        cout << currentCardName << " - " << i << endl;
    }
}

void processUserInput() {
    int cardValue;
    bool validInput = false;
    
    while (!validInput) {
        printGameRules();
        printAllCards();
        cout << endl << "Введите достоинство карты (от " << MIN_CARD_VALUE << " до " << MAX_CARD_VALUE << "): ";
        cin >> cardValue;

        if (isValidCard(cardValue)) {
            string cardName = getCardName(cardValue);
            cout << "Вы ввели: " << cardName << " (достоинство: " << cardValue << ")" << endl;
            validInput = true;
        } else {
            cout << "Ошибка: введено недопустимое значение карты!" << endl;
            cout << "Пожалуйста, попробуйте еще раз." << endl << endl;
        }
    }
}

bool askToContinue() {
    string choice;
    bool validChoice = false;
    
    while (!validChoice) {
        cout << "Продолжить ввод карт? (y/n): ";
        cin >> choice;
        cin.ignore(1000, '\n');
        
        if (choice == "y" || choice == "Y") return true;
        else if (choice == "n" || choice == "N") return false;
        else cout << "Пожалуйста, введите 'y' или 'n'." << endl;
    }
    return false;
}

int main() {
    initializeConsole();
    bool continueGame = true;
    
    while (continueGame) {
        processUserInput();
        cout << endl;
        continueGame = askToContinue();
        cout << endl;
    }

    cout << "Спасибо за игру!" << endl;
    return 0;
}