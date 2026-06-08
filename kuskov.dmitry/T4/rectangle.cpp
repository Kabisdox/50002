#include "rectangle.h"

float Rectangle::getArea() const
{
    return (width() * length());
}

Point Rectangle::getCentre() const
{
    Point centre(width()/2 + left_.x_, length()/2 + left_.y_);
    return centre;
}

void Rectangle::move(const float& movex, const float& movey)
{
    left_.move(movex, movey);
    right_.move(movex, movey);
}

void Rectangle::scale(const float& k)
{
    if (k <= 0)
    throw;
    Point centre = getCentre();
    float a = width();
    float b = length();
    left_.x_ = centre.x_ - a * k /2;
    right_.x_ = centre.x_ + a * k/2;
    left_.y_ = centre.y_ - b * k /2;
    right_.y_ = centre.y_ + b * k /2;
}

std::string Rectangle::getName() const
{
    return "RECTANGLE";
}

void Rectangle::getBox(float &xmin, float &ymin, float &xmax, float &ymax) const
{
        xmin = left_.x_;
        xmax = right_.x_;
        ymin = left_.y_;
        ymax = right_.y_;
}

