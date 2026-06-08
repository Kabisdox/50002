#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>

class Shape
{
protected:
    Shape() = default;
    virtual ~Shape() = default;

public:

    virtual double getArea() const = 0;

    virtual Point getCenter() const = 0;

    virtual void move(double x, double y) = 0;

    virtual void scale(double k) = 0;

    virtual std::string getName() const = 0;

    virtual double getLeft() const = 0;
    virtual double getRight() const = 0;
    virtual double getBottom() const = 0;
    virtual double getTop() const = 0;

};



#endif // SHAPE_H
