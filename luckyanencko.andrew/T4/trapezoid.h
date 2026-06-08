#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "shape.h"
#include "point.h"
#include <string>

class Trapezoid : public Shape {
public:
    Trapezoid(const Point& bottomLeft, double botLength, double topLength, double height);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    std::string getName() const override;

private:
    Point bottomLeft_;
    double botLength_;
    double topLength_;
    double height_;
};

#endif
