#ifndef NUMBER_SEQUENCE_H
#define NUMBER_SEQUENCE_H

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;

const double SEQUENCE_START = 25.0;
const double SEQUENCE_END = 35.0;
const double SEQUENCE_STEP = 1.0;
const double OFFSET_PLUS = 0.5;
const double OFFSET_MINUS = 0.2;

void printSequenceWithFor();
void printSequenceWithWhile(); 
void printSequenceWithDoWhile();

#endif