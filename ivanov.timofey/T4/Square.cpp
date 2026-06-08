#include "Square.h"
#include <stdexcept>
#include <cmath>

#include<iostream>

Square::Square(const Point& bottomLeft, double side)
    : bottomLeft_(bottomLeft), side_(side)
{
    if (side_ <= 0) {
        throw std::invalid_argument("Square side must be positive");
    }
}

double Square::getArea() const
{
    return side_ * side_;
}

Point Square::getCenter() const
{
    double centerX = bottomLeft_.x_ + side_ / 2.0;
    double centerY = bottomLeft_.y_ + side_ / 2.0;
    return Point(centerX, centerY);
}

void Square::move(double dx, double dy)
{
    bottomLeft_.x_ += dx;
    bottomLeft_.y_ += dy;
}

void Square::scale(double factor)
{
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }

    Point center = getCenter();

    side_ *= factor;

    bottomLeft_.x_ = center.x_ - side_ / 2.0;
    bottomLeft_.y_ = center.y_ - side_ / 2.0;
}

std::string Square::getName() const
{
    return "Square";
}

void Square::getBoundingBox(Point& bottomLeft, Point& topRight) const
{
    bottomLeft = bottomLeft_;
    topRight = Point(bottomLeft_.x_ + side_, bottomLeft_.y_ + side_);
}

void Square::print() const
{
    Point center = getCenter();
    std::cout << "[" << getName()
        << ", (" << center.x_ << ", " << center.y_ << "), "
        << getArea() << "]";
}
