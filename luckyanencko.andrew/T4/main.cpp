#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

#include "rectangle.h"
#include "trapezoid.h"
#include "composite_shape.h"

int main() {
    std::cout << std::fixed << std::setprecision(2);

    auto compShape = std::make_unique<CompositeShape>();

    compShape->addShape(std::make_unique<Rectangle>(
        Point{ 0.0, 0.0 }, Point{ 2.0, 2.0 }));

    compShape->addShape(std::make_unique<Trapezoid>(
        Point{ 1.0, 1.0 }, 4.0, 2.0, 2.0));

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(
        Point{ 0.0, 0.0 }, Point{ 4.0, 2.0 }));

    shapes.push_back(std::make_unique<Rectangle>(
        Point{ 1.0, 1.0 }, Point{ 5.0, 4.0 }));

    shapes.push_back(std::make_unique<Trapezoid>(
        Point{ 0.0, 0.0 }, 4.0, 2.0, 3.0));

    shapes.push_back(std::make_unique<Trapezoid>(
        Point{ 2.0, 2.0 }, 6.0, 4.0, 2.0));

    shapes.push_back(std::move(compShape));

    std::cout << "\n";

    for (const auto& shape : shapes) {
        Point center = shape->getCenter();

        if (shape->getName() == "COMPOSITE") {
            std::cout << "[COMPOSITE, (" << center.x << ", " << center.y
                << "), " << shape->getArea() << ":\n";

            auto comp = dynamic_cast<CompositeShape*>(shape.get());

            if (comp) {
                for (const auto& inner : comp->getShapes()) {
                    Point innerCenter = inner->getCenter();

                    std::cout << "  [" << inner->getName() << ", ("
                        << innerCenter.x << ", " << innerCenter.y
                        << "), " << inner->getArea() << "]\n";
                }
            }

            std::cout << "]\n";
        }
        else {
            std::cout << "[" << shape->getName() << ", ("
                << center.x << ", " << center.y << "), "
                << shape->getArea() << "]\n";
        }
    }

    for (auto& shape : shapes) {
        shape->scale(2.0);
    }

    std::cout << "\n";

    for (const auto& shape : shapes) {
        Point center = shape->getCenter();

        if (shape->getName() == "COMPOSITE") {
            std::cout << "[COMPOSITE, (" << center.x << ", " << center.y
                << "), " << shape->getArea() << ":\n";

            auto comp = dynamic_cast<CompositeShape*>(shape.get());

            if (comp) {
                for (const auto& inner : comp->getShapes()) {
                    Point innerCenter = inner->getCenter();

                    std::cout << "  [" << inner->getName() << ", ("
                        << innerCenter.x << ", " << innerCenter.y
                        << "), " << inner->getArea() << "]\n";
                }
            }

            std::cout << "]\n";
        }
        else {
            std::cout << "[" << shape->getName() << ", ("
                << center.x << ", " << center.y << "), "
                << shape->getArea() << "]\n";
        }
    }

    return 0;
}
