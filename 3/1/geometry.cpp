#include "geometry.h"
#include <cmath>
using namespace std;

bool is_point_on_circle1(double x, double y) {
    double distance = sqrt(x * x + y * y);
    return fabs(distance - RADIUS_1) < EPSILON;
}

bool is_point_on_circle2(double x, double y) {
    double distance = sqrt(x * x + y * y);
    return fabs(distance - RADIUS_2) < EPSILON;
}

bool is_point_on_y_axis(double x) {
    return fabs(x - X_BOUNDARY) < EPSILON;
}

bool is_point_on_horizontal_boundary(double y) {
    return fabs(y - Y_MIN) < EPSILON || fabs(y - Y_MAX) < EPSILON;
}

bool is_point_inside_area(double x, double y) {
    if (y < Y_MIN - EPSILON || y > Y_MAX + EPSILON) return false;
    if (x < X_BOUNDARY - EPSILON) return false;
    
    double distance = sqrt(x * x + y * y);
    return (distance >= RADIUS_1 - EPSILON) && (distance <= RADIUS_2 + EPSILON);
}