#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <locale>

#define FMT_HEADER_ONLY
#include "../vcpkg/fmt/fmt/include/fmt/format.h"

void printProgramDescription();
void printResultMessage(const std::string& result, double x, double y);
void drawCoordinateSystem(double userX, double userY);
void printLegend(double userX, double userY);
std::string getGridCharacter(double x, double y, double userX, double userY);

#endif