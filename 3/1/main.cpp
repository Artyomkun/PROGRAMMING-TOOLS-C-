#include <iostream>
#include <cmath>
#include <locale>
#include "geometry.h"
#include "visualization.h"

using namespace std; 

int main() {
    setlocale(LC_ALL, "Russian");
    
    double pointX;
    double pointY;
    
    printProgramDescription();
    
    cout << "Введите координату x: ";
    cin >> pointX;
    
    cout << "Введите координату y: ";
    cin >> pointY;

    cout << endl << "ВИЗУАЛИЗАЦИЯ:" << endl;
    drawCoordinateSystem(pointX, pointY);

    if (pointY < Y_MIN - EPSILON || pointY > Y_MAX + EPSILON) {
        printResultMessage("Нет", pointX, pointY);
        return 0;
    }

    if (pointX < X_BOUNDARY - EPSILON) {
        printResultMessage("Нет", pointX, pointY);
        return 0;
    }

    const double distance = sqrt(pointX * pointX + pointY * pointY);
    const bool inside_area = (distance >= RADIUS_1 - EPSILON) && (distance <= RADIUS_2 + EPSILON);

    if (inside_area) {
        const bool onCircle1 = is_point_on_circle1(pointX, pointY);
        const bool onCircle2 = is_point_on_circle2(pointX, pointY);
        const bool onYAxis = is_point_on_y_axis(pointX);
        const bool onHorizontal = is_point_on_horizontal_boundary(pointY);

        if (onCircle1 || onCircle2 || onYAxis || onHorizontal) {
            printResultMessage("На границе", pointX, pointY);
        } else {
            printResultMessage("Да", pointX, pointY);
        }
    } else {
        printResultMessage("Нет", pointX, pointY);
    }

    return 0;
}