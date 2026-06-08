#include <iostream>
#include "rectangle.h"

Rectangle::Rectangle(Point lb, Point rt) : leftBottom(lb), rightTop(rt) {}

double Rectangle::getArea() const {
    return (rightTop.x - leftBottom.x) * (rightTop.y - leftBottom.y);
}

Point Rectangle::getCenter() const {
    return {
        (leftBottom.x + rightTop.x) / 2,
        (leftBottom.y + rightTop.y) / 2
    };
}

void Rectangle::move(double dx, double dy) {
    leftBottom.x += dx;
    leftBottom.y += dy;
    rightTop.x += dx;
    rightTop.y += dy;
}

void Rectangle::scale(double factor) {
    if (factor <= 0) throw std::invalid_argument("Factor must be positive.");
    Point c = getCenter();

    leftBottom.x = c.x + (leftBottom.x - c.x) * factor;
    leftBottom.y = c.y + (leftBottom.y - c.y) * factor;

    rightTop.x = c.x + (rightTop.x - c.x) * factor;
    rightTop.y = c.y + (rightTop.y - c.y) * factor;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}
