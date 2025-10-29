#include "solver.h"
#include "system_info.h"

MathExpressionSolver::MathExpressionSolver() : x_(0.0), y_(0.0), result_(0.0) {}

void MathExpressionSolver::ClearScreen() {
    system(CLEAR_SCREEN);
}

void MathExpressionSolver::PrintProgramHeader() {
    ClearScreen();
    SystemInfo::PrintSystemInfo();
    
    std::cout << "==================================================" << std::endl;
    std::cout << "           MATHEMATICAL EXPRESSION SOLVER" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "        1 + sin(√(x+1))" << std::endl;
    std::cout << "f(x,y) = ----------------" << std::endl;
    std::cout << "         cos(12y-4)" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;
}

void MathExpressionSolver::GetUserInput() {
    std::cout << "Please provide input values:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    std::cout << "Enter x: ";
    while (!(std::cin >> x_)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Please enter a numeric value for x: ";
    }
    
    std::cout << "Enter y: ";
    while (!(std::cin >> y_)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Please enter a numeric value for y: ";
    }
}

bool MathExpressionSolver::ValidateInput() {
    if (x_ + 1 < 0) {
        DisplayErrorMessage("Square root argument must be non-negative (x + 1 >= 0)");
        return false;
    }
    
    double denominator = std::cos(12 * y_ - 4);
    if (std::fabs(denominator) < 1e-10) {
        DisplayErrorMessage("Division by zero (cos(12y - 4) ≈ 0)");
        return false;
    }
    
    return true;
}

void MathExpressionSolver::Calculate() {
    double numerator = 1 + std::sin(std::sqrt(x_ + 1));
    double denominator = std::cos(12 * y_ - 4);
    result_ = numerator / denominator;
}

void MathExpressionSolver::DisplayCalculationResult() {
    double square_root_value = std::sqrt(x_ + 1);
    double sine_value = std::sin(square_root_value);
    double numerator_value = 1 + sine_value;
    double cosine_argument = 12 * y_ - 4;
    double cosine_value = std::cos(cosine_argument);
    
    std::cout << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "                  SOLUTION" << std::endl;
    std::cout << "==================================================" << std::endl;
    
    std::cout << "Given:" << std::endl;
    std::cout << "x = " << x_ << std::endl;
    std::cout << "y = " << y_ << std::endl;
    std::cout << std::endl;
    
    std::cout << "Step-by-step solution:" << std::endl;
    std::cout << "1. √(x+1) = √(" << x_ << "+1) = " 
              << std::fixed << std::setprecision(4) << square_root_value << std::endl;
    std::cout << "2. sin(√(x+1)) = sin(" << square_root_value << ") = " 
              << sine_value << std::endl;
    std::cout << "3. 1 + " << sine_value << " = " 
              << numerator_value << std::endl;
    std::cout << std::endl;
    std::cout << "4. 12y-4 = 12*" << y_ << "-4 = " 
              << cosine_argument << std::endl;
    std::cout << "5. cos(12y-4) = cos(" << cosine_argument << ") = " 
              << cosine_value << std::endl;
    std::cout << std::endl;
    std::cout << "6. Result: " << std::endl;
    std::cout << "   " << std::setprecision(4) << numerator_value << std::endl;
    std::cout << "   -------------- = " << std::setprecision(6) << result_ << std::endl;
    std::cout << "   " << std::setprecision(4) << cosine_value << std::endl;
    std::cout << "==================================================" << std::endl;
}

void MathExpressionSolver::DisplayErrorMessage(const char* error_message) {
    std::cout << std::endl;
    std::cout << "ERROR: " << error_message << std::endl;
    std::cout << "Please adjust your input values." << std::endl;
    
    #ifdef _WIN32
        Sleep(3000);
    #else
        sleep(3);
    #endif
}

bool MathExpressionSolver::AskForAnotherCalculation() {
    char choice;
    
    while (true) {
        std::cout << std::endl;
        std::cout << "Do you want to perform another calculation? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(10000, '\n');

        if (choice == 'y' || choice == 'Y') {
            return true;
        }
        else if (choice == 'n' || choice == 'N') {
            return false;
        }
        else {
            
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << std::endl;
        }
    }
}

void MathExpressionSolver::Run() {
    bool continue_calculations = true;
    
    while (continue_calculations) {
        PrintProgramHeader();
        GetUserInput();
        
        if (!ValidateInput()) {
            continue;
        }
        
        Calculate();
        DisplayCalculationResult();
        
        continue_calculations = AskForAnotherCalculation();
    }
    
    std::cout << std::endl;
    std::cout << "Thank you for using Math Expression Solver!" << std::endl;
}