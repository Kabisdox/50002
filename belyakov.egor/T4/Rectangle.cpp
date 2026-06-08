#include "rectangle.h"

Rectangle::Rectangle(Point left, Point right) :
    width_(std::abs(right.x_ - left.x_)),
    height_(std::abs(right.y_ - left.y_)),
    centre_((left.x_ + right.x_) / 2, (left.y_ + right.y_) / 2)
{
}

double Rectangle::getArea() const
{
    return width_ * height_;
}

Point Rectangle::getCenter() const
{
    return centre_;
}

void Rectangle::move(double deltaX, double deltaY)
{
    centre_.x_ += deltaX;
    centre_.y_ += deltaY;
}

void Rectangle::scale(double k)
{
    if (k <= 0)
    {
        throw std::invalid_argument("Scale k must be positive");
    }

    width_ *= k;
    height_ *= k;
}

std::string Rectangle::getName() const
{
    return "RECTANGLE";
}

double Rectangle::getWidth() const
{
    return width_;
}

double Rectangle::getHeight() const
{
    return height_;
}

double Rectangle::getLeft() const
{
    return centre_.x_ - width_ / 2.0;
}

double Rectangle::getRight() const
{
    return centre_.x_ + width_ / 2.0;
}

double Rectangle::getBottom() const
{
    return centre_.y_ - height_ / 2.0;
}

double Rectangle::getTop() const
{
    return centre_.y_ + height_ / 2.0;
}
