#include <iostream>
#include "rightTrapezoid.h"

RightTrapezoid::RightTrapezoid(Point lb, double a, double b, double h)
    : leftBottom(lb), bottomBase(a), topBase(b), height(h) {}

double RightTrapezoid::getArea() const {
    return (bottomBase + topBase) / 2 * height;
}

Point RightTrapezoid::getCenter() const {
    double midBase = (bottomBase + topBase) / 2;

    return {
        leftBottom.x + midBase / 2,
        leftBottom.y + height / 2
    };
}

void RightTrapezoid::move(double dx, double dy) {
    leftBottom.x += dx;
    leftBottom.y += dy;
}

void RightTrapezoid::scale(double factor) {
    if (factor <= 0) throw std::invalid_argument("Factor must be positive.");
    Point c = getCenter();

    bottomBase *= factor;
    topBase *= factor;
    height *= factor;
    double midBase = (bottomBase + topBase) / 2;

    leftBottom.x = c.x - midBase / 2;
    leftBottom.y = c.y - height / 2;
}

std::string RightTrapezoid::getName() const {
    return "RIGHT_TRAPEZOID";
}
