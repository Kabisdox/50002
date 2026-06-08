#include "rectangle.h"
#include <stdexcept>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight)
    : bottomLeft_(bottomLeft), topRight_(topRight) {
    if (bottomLeft_.x >= topRight_.x || bottomLeft_.y >= topRight_.y) {
        throw std::invalid_argument("Invalid rectangle coordinates");
    }
}

double Rectangle::getArea() const {
    double width = topRight_.x - bottomLeft_.x;
    double height = topRight_.y - bottomLeft_.y;
    return width * height;
}

Point Rectangle::getCenter() const {
    double centerX = bottomLeft_.x + (topRight_.x - bottomLeft_.x) / 2.0;
    double centerY = bottomLeft_.y + (topRight_.y - bottomLeft_.y) / 2.0;
    return {centerX, centerY};
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double coefficient) {
    if (coefficient <= 0) {
        throw std::invalid_argument("Invalid coefficient");
    }

    Point center = getCenter();
    double width = (topRight_.x - bottomLeft_.x) * coefficient;
    double height = (topRight_.y - bottomLeft_.y) * coefficient;

    bottomLeft_.x = center.x - width / 2.0;
    bottomLeft_.y = center.y - height / 2.0;
    topRight_.x = center.x + width / 2.0;
    topRight_.y = center.y + height / 2.0;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}
