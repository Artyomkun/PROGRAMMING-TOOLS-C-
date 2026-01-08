#ifndef GEOMETRY_H
#define GEOMETRY_H

const double Y_MIN = -3.0;
const double Y_MAX = 3.0;
const double RADIUS_1 = 4.0; 
const double RADIUS_2 = 6.0; 
const double X_BOUNDARY = 0.0;
const double EPSILON = 1e-9;

bool is_point_on_circle1(double x, double y);
bool is_point_on_circle2(double x, double y);
bool is_point_on_y_axis(double x);
bool is_point_on_horizontal_boundary(double y);
bool is_point_inside_area(double x, double y);

#endif