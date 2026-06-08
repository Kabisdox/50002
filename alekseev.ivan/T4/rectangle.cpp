#include "rectangle.h"
#include <cmath>

Rectangle::Rectangle(const Point& leftBottom, const Point& rightTop)
{
    leftBottom_ = leftBottom;
    rightTop_ = rightTop;
}

double Rectangle::getArea() const
{
    double width = std::fabs(rightTop_.x - leftBottom_.x);
    double height = std::fabs(rightTop_.y - leftBottom_.y);
    return width * height;
}

Point Rectangle::getCenter() const
{
    Point center;
    center.x = (leftBottom_.x + rightTop_.x) / 2.0;
    center.y = (leftBottom_.y + rightTop_.y) / 2.0;
    return center;
}

void Rectangle::move(double dx, double dy)
{
    leftBottom_.x += dx;
    leftBottom_.y += dy;
    rightTop_.x += dx;
    rightTop_.y += dy;
}

void Rectangle::scale(double factor)
{
    if (factor <= 0)
    {
        return;
    }

    Point center = getCenter();

    double halfWidth = std::fabs(rightTop_.x - leftBottom_.x) / 2.0;
    double halfHeight = std::fabs(rightTop_.y - leftBottom_.y) / 2.0;

    halfWidth *= factor;
    halfHeight *= factor;

    leftBottom_.x = center.x - halfWidth;
    leftBottom_.y = center.y - halfHeight;
    rightTop_.x = center.x + halfWidth;
    rightTop_.y = center.y + halfHeight;
}

std::string Rectangle::getName() const
{
    return "RECTANGLE";
}
