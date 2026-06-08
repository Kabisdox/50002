#include <iostream>
#include <vector>
#include <iomanip>
#include "CompositeShape.h"
int main() {
    std::setlocale(LC_ALL, "Russian");
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(0, 0), Point(2, 4)));
    shapes.push_back(std::make_unique<Square>(Point(10, 10), 5));
    shapes.push_back(std::make_unique<Rectangle>(Point(-5, -5), Point(-2, -2)));
    shapes.push_back(std::make_unique<Square>(Point(0, 10), 2));

    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Rectangle>(Point(0, 0), Point(2, 2)));
    composite->addShape(std::make_unique<Square>(Point(4, 4), 2));

    shapes.push_back(std::move(composite));

    std::cout << "ДО МАСШТАБИРОВАНИЯ" << "\n";
    for (const auto& shape : shapes) {
        shape->print();
        std::cout << std::endl;
    }

    double k = 2.0;
    for (auto& shape : shapes) {
        shape->scale(k);
    }

    std::cout << "\n--- ПОСЛЕ МАСШТАБИРОВАНИЯ (k=2.0)" << "\n";
    for (const auto& shape : shapes) {
        shape->print();
        std::cout << std::endl;
    }
}
