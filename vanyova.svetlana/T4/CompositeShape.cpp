#include "CompositeShape.h"
#include <algorithm>
#include <iomanip>
#include <limits>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Shape pointer null!!!");
    }
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double resultArea = 0;
    for (auto& shape : shapes_) {
        resultArea += shape->getArea();
    }
    return resultArea;
}
std::string CompositeShape::getName() const {
    return "Composite Shape";
}
Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        return Point(0, 0);
    }
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();
    for (auto& shape : shapes_) {
        Point oldCenter = shape->getCenter();
        minX = std::min(minX, oldCenter.x_);
        minY = std::min(minY, oldCenter.y_);
        maxX = std::max(maxX, oldCenter.x_);
        maxY = std::max(maxY, oldCenter.y_);
    }
    return Point((minX + maxX) / 2, (minY + maxY) / 2);
}
void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}
void CompositeShape::scale(double k) {
    if (k <= 0) {
        throw std::invalid_argument("factor must be positive");
    };
    if (shapes_.empty()) {
        throw std::runtime_error("Composite shape is empty");
    }
    if (k == 1) {
        return;
    }
    Point compositeCenter = getCenter();
    for (auto& shape : shapes_) {
        Point oldCenter = shape->getCenter();
        double dx = oldCenter.x_ - compositeCenter.x_;
        double dy = oldCenter.y_ - compositeCenter.y_;
        double shiftX = dx * (k - 1);
        double shiftY = dy * (k - 1);
        shape->move(shiftX, shiftY);
        shape->scale(k);
    }
}
void CompositeShape::print() const {
    std::cout << '[' << "COMPOSITE" << ", " << "(" << getCenter().x_ << ", "
        << std::setprecision(2) << getCenter().y_ << ")"
        << ", " << getArea() << ":\n";
    for (size_t i = 0; i < shapes_.size(); ++i) {
        std::cout << " ";
        shapes_[i]->printRaw();
        if (i < shapes_.size() - 1) {
            std::cout << ",";
        }
        std::cout << "\n";
    }
    std::cout << "]" << std::endl;
};
void CompositeShape::printRaw() const {
    this->print();
}
