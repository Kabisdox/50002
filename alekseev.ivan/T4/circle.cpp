#include "circle.h"

Circle::Circle(const Point& center, double radius)
{
    center_ = center;
    radius_ = radius;
}

double Circle::getArea() const
{
    const double PI = 3.14159265358979323846;
    return PI * radius_ * radius_;
}

Point Circle::getCenter() const
{
    return center_;
}

void Circle::move(double dx, double dy)
{
    center_.x += dx;
    center_.y += dy;
}

void Circle::scale(double factor)
{
    if (factor <= 0)
    {
        return;
    }

    radius_ *= factor;
}

std::string Circle::getName() const
{
    return "CIRCLE";
}
