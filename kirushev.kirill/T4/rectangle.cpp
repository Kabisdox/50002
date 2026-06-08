#include "rectangle.h"
#include <stdexcept>

double Rectangle::getArea() const {
    double width = topRight_.x - bottomLeft_.x;
    double height = topRight_.y - bottomLeft_.y;
    return width * height;
}

Point Rectangle::getCenter() const {
    return Point(
        (bottomLeft_.x + topRight_.x) / 2,
        (bottomLeft_.y + topRight_.y) / 2
    );
}

void Rectangle::move(const double dx, const double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;

    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(const double k) {
    if (k <= 0) {
        throw std::invalid_argument("The factor must be greater than 0.");
    }

    Point center = getCenter();

    bottomLeft_.x = center.x + (bottomLeft_.x - center.x) * k;
    bottomLeft_.y = center.y + (bottomLeft_.y - center.y) * k;

    topRight_.x = center.x + (topRight_.x - center.x) * k;
    topRight_.y = center.y + (topRight_.y - center.y) * k;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}
