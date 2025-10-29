#include "geometry.h"

bool is_point_on_parabola(double x, double y) {
    double parabolaX = y * y / 4.0 - 5.0;
    return fabs(x - parabolaX) < EPSILON;
}

bool is_point_on_circle(double x, double y) {
    if (fabs(y) > RADIUS + EPSILON) return false;
    double circleX = sqrt(RADIUS * RADIUS - y * y);
    return fabs(x - circleX) < EPSILON;
}

bool is_point_on_horizontal_boundary(double y) {
    return fabs(y - Y_MIN) < EPSILON || fabs(y - Y_MAX) < EPSILON;
}

bool is_point_inside_area(double x, double y) {
    if (y < Y_MIN - EPSILON || y > Y_MAX + EPSILON) return false;
    
    double parabolaBound = y * y / 4.0 - 5.0;
    if (x < parabolaBound + EPSILON) return false;
    
    double circleBound = sqrt(RADIUS * RADIUS - y * y);
    if (x > circleBound - EPSILON) return false;
    
    return true;
}