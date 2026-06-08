#include "compositeShape.h"

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double area = 0;
    for (auto& s : shapes)
        area += s->getArea();
    return area;
}

Point CompositeShape::getCenter() const {
    double minX = shapes[0]->getCenter().x;
    double maxX = minX;
    double minY = shapes[0]->getCenter().y;
    double maxY = minY;

    for (auto& s : shapes) {
        Point c = s->getCenter();

        if (c.x < minX) minX = c.x;
        if (c.x > maxX) maxX = c.x;
        if (c.y < minY) minY = c.y;
        if (c.y > maxY) maxY = c.y;
    }

    return { (minX + maxX) / 2, (minY + maxY) / 2 };
}

void CompositeShape::move(double dx, double dy) {
    for (auto& s : shapes)
        s->move(dx, dy);
}

void CompositeShape::scale(double factor) {
    Point center = getCenter();

    for (auto& s : shapes) {
        Point c = s->getCenter();

        double dx = (c.x - center.x) * (factor - 1);
        double dy = (c.y - center.y) * (factor - 1);

        s->move(dx, dy);
        s->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

const std::vector<std::unique_ptr<Shape>>& CompositeShape::getShapes() const {
    return shapes;
}
