#ifndef DIGIT_SUM_CHECKER_H
#define DIGIT_SUM_CHECKER_H

class DigitSumChecker {
private:
    int m_number;
    int m_digitSum;
    bool m_isMultipleOfSeven;

public:
    DigitSumChecker();
    void Run();
    
private:
    void InitializeConsole();
    void DisplayWelcomeMessage();
    void GetUserInput();
    bool ValidateInput() const;
    void CalculateDigitSum();
    void CheckMultipleOfSeven();
    void DisplayResults() const;
    bool AskForAnotherCalculation();
};

#endif