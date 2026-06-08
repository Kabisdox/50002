#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include "rectangle.h"
#include "trapezoid.h"
#include "compositeShape.h"

void printShape(const Shape& shape, int indent = 0)
{
    std::cout << std::fixed << std::setprecision(2);
    Point centre = shape.getCentre();
    float area = shape.getArea();
    std::cout << std::string(indent, ' ');
    if (shape.getName() == "COMPOSITE")
    {
        std::cout << std::string(indent, ' ') << "[COMPOSITE, (" << centre.x_ << ", " << centre.y_ << "), " << area << ":\n";
        const CompositeShape& comp = dynamic_cast<const CompositeShape&>(shape);
        for (size_t i = 0; i < comp.getShapes().size(); ++i)
        {
            printShape(*comp.getShapes()[i], indent + 2);
            if (i != comp.getShapes().size() - 1) std::cout << ",\n";
            else std::cout << "\n";
        }
        std::cout << std::string(indent, ' ') << "]\n";
    }
    else
    {
        std::cout << "[" << shape.getName() << ", (" << centre.x_ << ", " << centre.y_ << "), " << area << "]";
    }
}

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Rectangle>(Point{1, 1}, Point{4, 3}));
    shapes.push_back(std::make_unique<Trapezoid>(Point{0, 0}, 6, 4, 3));
    shapes.push_back(std::make_unique<Rectangle>(Point{5, 2}, Point{7, 5}));
    shapes.push_back(std::make_unique<Trapezoid>(Point{2, 4}, 5, 3, 2));
    auto composite = std::make_unique<CompositeShape>();
    composite->add(std::make_unique<Rectangle>(Point{10, 10}, Point{12, 12}));
    composite->add(std::make_unique<Trapezoid>(Point{8, 8}, 4, 2, 1.5));
    shapes.push_back(std::move(composite));
    std::cout << "До масштабирования (factor = 1):\n";
    for (const auto& shape : shapes)
    {
        printShape(*shape);
        std::cout << "\n";
    }
    for (auto& shape : shapes)
    {
        shape->scale(2.0);
    }
    std::cout << "\nПосле масштабирования (factor = 2):\n";
    for (const auto& shape : shapes)
    {
        printShape(*shape);
        std::cout << "\n";
    }
    return 0;
}

