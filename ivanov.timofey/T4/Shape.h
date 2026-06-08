#ifndef SHAPE_H
#define SHAPE_H
#include <string>
#include "Point.h"

class Shape {
public:
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
    virtual Point getCenter() const = 0;
    virtual void move(double x, double y) = 0;
    virtual void scale(double k) = 0;
    virtual std::string getName() const = 0;
    virtual void getBoundingBox(Point& bottomLeft, Point& topRight) const = 0;
    virtual void print() const = 0;
};

#endif
