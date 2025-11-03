#ifndef SEQUENCE_CALCULATOR_H
#define SEQUENCE_CALCULATOR_H

class SequenceCalculator {
public:
    SequenceCalculator(long double firstTerm = 1.0L, long double secondTerm = 2.0L);
    
    long double calculateTerm(int termNumber) const;
    void displaySequenceInfo() const;
    void printSequenceUpTo(int maxTerm) const;

private:
    const long double FIRST_TERM;
    const long double SECOND_TERM;
};

#endif