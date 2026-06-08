#include <iostream>
#include <iomanip>

#include "point.h"
#include "shape.h"
#include "rectangle.h"
#include "square.h"
#include "compositeShape.h"

void printShape(const std::unique_ptr<Shape>& shape) {
    Point c = shape->getCenter();
    auto comp = dynamic_cast<CompositeShape*>(shape.get());

    if (comp) {
        printf("[%s, (%.2f, %.2f), %.2f:\n", shape->getName().c_str(), c.x, c.y, shape->getArea());
        for (const auto &s : comp->getShapes()) {
            Point inner = s->getCenter();
            printf(" %s, (%.2f, %.2f), %.2f,\n", s->getName().c_str(), inner.x, inner.y, s->getArea());
        }
        printf("]\n");
    } else {
        printf("[%s, (%.2f, %.2f), %.2f]\n", shape->getName().c_str(), c.x, c.y, shape->getArea());
    }
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(0,0), Point(4,2)));
    shapes.push_back(std::make_unique<Rectangle>(Point(2,2), Point(6,5)));
    shapes.push_back(std::make_unique<Square>(Point(0, 0), 4));
    shapes.push_back(std::make_unique<Rectangle>(Point(5,-1), Point(8,1)));

    auto comp = std::make_unique<CompositeShape>();

    comp->addShape(std::make_unique<Rectangle>(Point(10,10), Point(12,12)));
    comp->addShape(std::make_unique<Square>(Point(4,10), 3));

    shapes.push_back(std::move(comp));

    std::cout << "Before scaling:\n";
    for (const auto& shape : shapes)
        printShape(shape);

    for (const auto& shape : shapes)
        shape->scale(2);

    std::cout << "\nAfter scaling:\n";
    for (const auto& s : shapes)
        printShape(s);

    return 0;
}
