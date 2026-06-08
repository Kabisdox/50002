#ifndef POINT_H
#define POINT_H
#include <iostream>
struct Point
{
    Point() :
        x_(0.0),
        y_(0.0)
    {}
    Point(double x, double y) :
        x_(x),
        y_(y)
    {}
    double x_;
    double y_;
};
#endif // !POINT_H
