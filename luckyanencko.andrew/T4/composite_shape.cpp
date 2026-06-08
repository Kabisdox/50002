#include "composite_shape.h"
#include <stdexcept>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Null shape");
    }
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double total = 0.0;
    for (const auto& shape : shapes_) {
        total += shape->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        return { 0.0, 0.0 };
    }

    double sumX = 0.0;
    double sumY = 0.0;

    for (const auto& shape : shapes_) {
        Point c = shape->getCenter();
        sumX += c.x;
        sumY += c.y;
    }

    return { sumX / shapes_.size(), sumY / shapes_.size() };
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Invalid scale factor");
    }

    Point center = getCenter();

    for (auto& shape : shapes_) {
        Point c = shape->getCenter();

        double dx = (c.x - center.x) * (factor - 1);
        double dy = (c.y - center.y) * (factor - 1);

        shape->move(dx, dy);
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

const std::vector<std::unique_ptr<Shape>>& CompositeShape::getShapes() const {
    return shapes_;
}

