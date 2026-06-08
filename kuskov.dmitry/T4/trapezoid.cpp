#include "trapezoid.h"

float Trapezoid::getArea() const
{
    return (top_ + bottom_)/2 * height_;
}

Point Trapezoid::getCentre() const
{
    Point centre(left_.x_ + bottom_/2, left_.y_ + height_/2);
    return centre;
}

void Trapezoid::move(const float& movex, const float& movey)
{
    left_.move(movex, movey);
}

void Trapezoid::scale(const float& k)
{
    if (k <= 0)
    throw;
    Point centre = getCentre();
    bottom_ *= k;
    top_ *= k;
    height_ *= k;
    left_.x_ = centre.x_ - bottom_/2;
    left_.y_ = centre.y_ - height_/2;
}

std::string Trapezoid::getName() const
{
    return "TRAPEZOID";
}

void Trapezoid::getBox(float &xmin, float &ymin, float &xmax, float &ymax) const
{
        xmin = left_.x_;
        xmax = left_.x_ + bottom_;
        ymin = left_.y_;
        ymax = left_.y_ + height_;
}

