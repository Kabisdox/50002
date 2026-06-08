#include "rectangle.h"
Rectangle::Rectangle() :
    pointA_(0, 0),
    pointB_(0, 0)
{}
Rectangle::Rectangle(Point pointA, Point pointB) :
    pointA_(pointA),
    pointB_(pointB)
{}
double Rectangle::getArea() const {
    double width = pointB_.x_ - pointA_.x_;
    double height =pointB_.y_ - pointA_.y_;
    return width * height;
}
Point Rectangle::getCenter() const {
    double centerX = pointA_.x_ + (pointB_.x_ - pointA_.x_) / 2;
    double centerY = pointA_.y_ + (pointB_.y_ - pointA_.y_) / 2;
    return Point(centerX, centerY);
}
void Rectangle::move(double dx, double dy) {
    pointA_.x_ += dx;
    pointA_.y_ += dy;
    pointB_.x_ += dx;
    pointB_.y_ += dy;
}
void Rectangle::scale(double k) {
    if (k <= 0) {
        throw std::invalid_argument("factor must be positive");
    };
    Point center = getCenter();
    double newWidth = (pointB_.x_ - pointA_.x_) * k;
    double newHeight = (pointB_.y_ - pointA_.y_) * k;
    pointA_.x_ = center.x_ - newWidth / 2;
    pointA_.y_ = center.y_ - newHeight / 2;
    pointB_.x_ = center.x_ + newWidth / 2;
    pointB_.y_ = center.y_ + newHeight / 2;
}
std::string Rectangle::getName() const {
    return "Rectangle";
}
void Rectangle::print() const {
    std::cout << '[';
    printRaw();
    std::cout << ']';
}
void Rectangle::printRaw() const {
    std::cout << "RECTANGLE, (" << std::fixed << std::setprecision(2)
        << getCenter().x_ << ", " << getCenter().y_ << "), " << getArea();
}
