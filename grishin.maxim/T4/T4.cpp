#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

#include "rectangle.h"
#include "rightTrapezoid.h"
#include "compositeShape.h"

void printShape(const Shape& s) {
    Point c = s.getCenter();

    std::cout << "[" << s.getName()
        << ", (" << std::fixed << std::setprecision(2)
        << c.x << ", " << c.y << "), "
        << s.getArea() << "]";
}

void printComposite(const CompositeShape& comp) {
    Point c = comp.getCenter();

    std::cout << "[COMPOSITE, ("
        << std::fixed << std::setprecision(2)
        << c.x << ", " << c.y << "), "
        << comp.getArea() << ":\n";

    for (const auto& s : comp.getShapes()) {
        Point sc = s->getCenter();

        std::cout << "  " << s->getName()
            << ", (" << sc.x << ", " << sc.y << "), "
            << s->getArea() << ",\n";
    }

    std::cout << "]\n";
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point{ 0, 0 }, Point{ 4, 3 }));
    shapes.push_back(std::make_unique<RightTrapezoid>(Point{ 0, 0 }, 6, 4, 3));
    shapes.push_back(std::make_unique<Rectangle>(Point{ 1, 1 }, Point{ 5, 4 }));
    shapes.push_back(std::make_unique<RightTrapezoid>(Point{ 2, 2 }, 8, 5, 4));

    auto comp = std::make_unique<CompositeShape>();
    comp->addShape(std::make_unique<Rectangle>(Point{ 0, 0 }, Point{ 2, 2 }));
    comp->addShape(std::make_unique<RightTrapezoid>(Point{ 1, 1 }, 4, 2, 2));
    comp->addShape(std::make_unique<Rectangle>(Point{ 3, 3 }, Point{ 6, 5 }));

    shapes.push_back(std::move(comp));

    std::cout << "Before scaling:\n";

    for (const auto& s : shapes) {
        if (s->getName() == "COMPOSITE") {
            printComposite(static_cast<const CompositeShape&>(*s));
        }
        else {
            printShape(*s);
            std::cout << "\n";
        }
    }

    for (auto& s : shapes) {
        s->scale(2);
    }

    std::cout << "\nAfter scaling:\n";

    for (const auto& s : shapes) {
        if (s->getName() == "COMPOSITE") {
            printComposite(static_cast<const CompositeShape&>(*s));
        }
        else {
            printShape(*s);
            std::cout << "\n";
        }
    }

    return 0;
}
