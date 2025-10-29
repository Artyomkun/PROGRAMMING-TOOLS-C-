#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

using std::fabs;
using std::sqrt;

const double Y_MIN = -3.0;
const double Y_MAX = 3.0;
const double RADIUS = 8.0;
const double EPSILON = 1e-9;

bool is_point_on_parabola(double x, double y);
bool is_point_on_circle(double x, double y);
bool is_point_on_horizontal_boundary(double y);
bool is_point_inside_area(double x, double y);

#endif