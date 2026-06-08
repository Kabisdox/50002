#include "Rectangle.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

Rectangle::Rectangle(const Point& bottomLeft, const Point& upperRight)
    : bottomLeft_(bottomLeft), upperRight_(upperRight) {}

double Rectangle::getArea() const
{
    double width = upperRight_.x_ - bottomLeft_.x_;
    double height = upperRight_.y_ - bottomLeft_.y_;
    return std::abs(width * height);
}

Point Rectangle::getCenter() const
{
    double centerX = (bottomLeft_.x_ + upperRight_.x_) / 2.0;
    double centerY = (bottomLeft_.y_ + upperRight_.y_) / 2.0;
    return Point(centerX, centerY);
}

void Rectangle::move(double x, double y)
{
    bottomLeft_.x_ += x;
    bottomLeft_.y_ += y;
    upperRight_.x_ += x;
    upperRight_.y_ += y;
}

void Rectangle::scale(double k)
{
    if (k <= 0)
    {
        throw std::invalid_argument("Scale factor must be positive");
    }

    Point center = getCenter();
    bottomLeft_.x_ = center.x_ + (bottomLeft_.x_ - center.x_) * k;
    bottomLeft_.y_ = center.y_ + (bottomLeft_.y_ - center.y_) * k;
    upperRight_.x_ = center.x_ + (upperRight_.x_ - center.x_) * k;
    upperRight_.y_ = center.y_ + (upperRight_.y_ - center.y_) * k;
}

std::string Rectangle::getName() const
{
    return "Rectangle";
}

void Rectangle::getBoundingBox(Point& bottomLeft, Point& topRight) const
{
    bottomLeft = bottomLeft_;
    topRight = upperRight_;
}

void Rectangle::print() const
{
    Point center = getCenter();
    std::cout << "[" << getName()
        << ", (" << center.x_ << ", " << center.y_ << "), "
        << getArea() << "]";
}
