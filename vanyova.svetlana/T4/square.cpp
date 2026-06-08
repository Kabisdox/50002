#include "square.h"
Square::Square() :
    top_(0, 0),
    lenght_(0)
{}
Square::Square(Point top, double lenght) :
    top_(top),
    lenght_(lenght)
{
    if (lenght <= 0) {
        throw std::invalid_argument("Length must be positive");
    }
}
double Square::getArea() const { return lenght_ * lenght_; };
Point Square::getCenter() const {
    double centerX = top_.x_ + lenght_ / 2;
    double centerY = top_.y_ + lenght_ / 2;
    return Point(centerX, centerY);
};
void Square::move(double dx, double dy) {
    top_.x_ += dx;
    top_.y_ += dy;
}
void Square::scale(double k) {
    if (k <= 0) {
        throw std::invalid_argument("factor must be positive");
    };
    Point center = getCenter();
    lenght_ *= k;
    top_.x_ = center.x_ - lenght_ / 2;
    top_.y_ = center.y_ - lenght_ / 2;
};
std::string Square::getName() const {
    return "Square";
}
void Square::print() const {
    std::cout << '[';
    printRaw();
    std::cout << ']';
}
void Square::printRaw() const {
    std::cout << "SQUARE, (" << std::fixed << std::setprecision(2)
        << getCenter().x_ << ", " << getCenter().y_ << "), " << getArea();
}
