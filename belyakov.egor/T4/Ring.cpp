#include "Ring.h"
const double PI = 3.141592;

Ring::Ring(Point centre, double radius1, double radius2) :
    radius1_(radius1),
    radius2_(radius2),
    centre_(centre)
{
}

double Ring::getArea() const
{
    return PI * (radius2_ * radius2_ - radius1_ * radius1_);
}

Point Ring::getCenter() const
{
    return centre_;
}

void Ring::move(double deltaX, double deltaY)
{
    centre_.x_ += deltaX;
    centre_.y_ += deltaY;
}

void Ring::scale(double k)
{
    if (k <= 0)
    {
        throw std::invalid_argument("Scale k must be positive");
    }

    radius1_ *= k;
    radius2_ *= k;
}

std::string Ring::getName() const
{
    return "RING";
}

double Ring::getRadius1() const
{
    return radius1_;
}

double Ring::getRadius2() const
{
    return radius2_;
}

double Ring::getLeft() const
{
    return centre_.x_ - radius2_;
}

double Ring::getRight() const
{
    return centre_.x_ + radius2_;
}

double Ring::getBottom() const
{
    return centre_.y_ - radius2_;
}

double Ring::getTop() const
{
    return centre_.y_ + radius2_;
}
