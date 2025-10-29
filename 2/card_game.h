#ifndef CARD_GAME_H
#define CARD_GAME_H

#include <iostream>
#include <string>
#include <windows.h>

using std::cout;
using std::endl;
using std::cin;
using std::string;

const int MIN_CARD_VALUE = 7;
const int MAX_CARD_VALUE = 13;
const int ACE_VALUE = 14;
const int KING_VALUE = 13;
const int QUEEN_VALUE = 12;
const int JACK_VALUE = 11;
const int TEN_VALUE = 10;

void initializeConsole();
string getCardName(int cardValue);
bool isValidCard(int cardValue);
void printGameRules();
void printAllCards();
void processUserInput();
bool askToContinue();

#endif