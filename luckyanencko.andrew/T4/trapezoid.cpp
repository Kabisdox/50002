#include "trapezoid.h"
#include <stdexcept>

Trapezoid::Trapezoid(const Point& bottomLeft, double botLength, double topLength, double height)
    : bottomLeft_(bottomLeft), botLength_(botLength), topLength_(topLength), height_(height) {
    if (botLength_ <= 0 || topLength_ <= 0 || height_ <= 0) {
        throw std::invalid_argument("Invalid dimensions");
    }
}

double Trapezoid::getArea() const {
    return ((botLength_ + topLength_) / 2.0) * height_;
}

Point Trapezoid::getCenter() const {
    double centerX = bottomLeft_.x + (botLength_ / 2.0);
    double centerY = bottomLeft_.y + (height_ / 2.0);
    return {centerX, centerY};
}

void Trapezoid::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
}

void Trapezoid::scale(double coefficient) {
    if (coefficient <= 0) {
        throw std::invalid_argument("Invalid coefficient");
    }

    Point center = getCenter();

    botLength_ *= coefficient;
    topLength_ *= coefficient;
    height_ *= coefficient;

    bottomLeft_.x = center.x - (botLength_ / 2.0);
    bottomLeft_.y = center.y - (height_ / 2.0);
}

std::string Trapezoid::getName() const {
    return "TRAPEZOID";
}
