#include <iostream>
#include <cmath>
#include <locale>
#include "geometry.h"
#include "visualization.h"

using std::cin;
using std::cout;
using std::endl;
using std::sqrt;

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

    const double parabolaBound = pointY * pointY / 4.0 - 5.0;
    const double circleBound = sqrt(RADIUS * RADIUS - pointY * pointY);

    if (pointX >= parabolaBound - EPSILON && pointX <= circleBound + EPSILON) {
        const bool onParabola = is_point_on_parabola(pointX, pointY);
        const bool onCircle = is_point_on_circle(pointX, pointY);
        const bool onHorizontal = is_point_on_horizontal_boundary(pointY);

        if (onParabola || onCircle || onHorizontal) {
            printResultMessage("На границе", pointX, pointY);
        } else {
            printResultMessage("Да", pointX, pointY);
        }
    } else {
        printResultMessage("Нет", pointX, pointY);
    }

    return 0;
}