#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "point.h"
#include <string>

class Rectangle : public Shape {
public:
    Rectangle(const Point& bottomLeft, const Point& topRight);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    std::string getName() const override;

private:
    Point bottomLeft_;
    Point topRight_;
};

#endif
