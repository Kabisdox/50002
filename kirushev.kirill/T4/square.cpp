#include "square.h"
#include <stdexcept>

double Square::getArea() const {
    return sideLength_ * sideLength_;
}

Point Square::getCenter() const {
    return Point(
        bottomLeft_.x + sideLength_ / 2,
        bottomLeft_.y + sideLength_ / 2
    );
}

void Square::move(const double dx, const double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
}

void Square::scale(const double k) {
    if (k <= 0) {
        throw std::invalid_argument("The factor must be greater than 0.");
    }

    Point center = getCenter();

    sideLength_ *= k;

    bottomLeft_.x = center.x - sideLength_ / 2;
    bottomLeft_.y = center.y - sideLength_ / 2;
}

std::string Square::getName() const {
    return "SQUARE";
}
