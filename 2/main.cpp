#ifdef _WIN32
    #include <windows.h>
    #define NOMINMAX
#endif

#include "min_finder.h"
#include "visual_finder.h"
#include "application.h"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>

using namespace std;

namespace {
    const int VISUAL_MODE = 1;
    const int INTERACTIVE_MODE = 2;
    const int EXIT_MODE = 3;
    const string PROGRAM_TITLE = "MINIMUM VALUE FINDER PROGRAM";
    const string SEPARATOR = "=================================";
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

void displayMainMenu() {
    cout << "\n" << SEPARATOR << endl;
    cout << PROGRAM_TITLE << endl;
    cout << SEPARATOR << endl;
    cout << "Select working mode:" << endl;
    cout << "1. Visual Mode (Graphical Interface)" << endl;
    cout << "2. Interactive Mode (Multiple Versions)" << endl;
    cout << "3. Exit Program" << endl;
    cout << "Your choice (1-3): ";
}

int getMenuChoice() {
    int choice;
    string input;
    
    while (true) {
        getline(cin, input);
        stringstream ss(input);
        
        if (ss >> choice && ss.eof() && choice >= 1 && choice <= 3) {
            return choice;
        } else {
            cout << "Invalid input! Please enter 1, 2 or 3: ";
        }
    }
}

void runVisualMode() {
    VisualMinFinder visual_finder;
    cout << "\n" << SEPARATOR << endl;
    cout << "VISUAL MODE" << endl;
    cout << SEPARATOR << endl;
    visual_finder.run();
}

void runInteractiveMode() {
    Application interactive_app;
    cout << "\n" << SEPARATOR << endl;
    cout << "INTERACTIVE MODE" << endl;
    cout << SEPARATOR << endl;
    interactive_app.run();
}

void processMainMenuChoice(int choice) {
    switch (choice) {
        case VISUAL_MODE:
            runVisualMode();
            break;
        case INTERACTIVE_MODE:
            runInteractiveMode();
            break;
        case EXIT_MODE:
            cout << "\nThank you for using the program! Goodbye!" << endl;
            break;
        default:
            cout << "Unexpected error! Please try again." << endl;
    }
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif
    
    cout << "Initializing Minimum Value Finder..." << endl;
    
    int main_choice;
    
    do {
        displayMainMenu();
        main_choice = getMenuChoice();
        processMainMenuChoice(main_choice);
        
    } while (main_choice != EXIT_MODE);
    
    cout << "\nPress Enter to exit...";
    clearInputBuffer();
    
    return 0;
}