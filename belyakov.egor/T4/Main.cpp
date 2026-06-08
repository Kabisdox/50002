#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "rectangle.h"
#include "Ring.h"
#include "CompositeShape.h"

void printShape(const Shape& shape)
{
    Point center = shape.getCenter();

    std::cout << "[" << shape.getName()
        << ", (" << std::fixed << std::setprecision(2)
        << center.x_ << ", " << center.y_ << ")"
        << ", " << shape.getArea() << "]";
}

void printComposite(const CompositeShape& composite)
{
    Point center = composite.getCenter();

    std::cout << "[" << composite.getName()
        << ", (" << std::fixed << std::setprecision(2)
        << center.x_ << ", " << center.y_ << ")"
        << ", " << composite.getArea() << ":\n";

    for (const auto& shape : composite.getShapes())
    {
        std::cout << " ";
        printShape(*shape);
        std::cout << '\n';
    }

    std::cout << "]";
}

int main()
{
    std::vector<std::shared_ptr<Shape>> shapes;

    shapes.push_back(std::make_shared<Rectangle>(Point{ 0.0, 0.0 }, Point{ 4.0, 2.0 }));
    shapes.push_back(std::make_shared<Rectangle>(Point{ 2.0, 1.0 }, Point{ 6.0, 5.0 }));
    shapes.push_back(std::make_shared<Ring>(Point{ 3.0, 3.0 }, 1.0, 2.0));
    shapes.push_back(std::make_shared<Ring>(Point{ -2.0, 2.0 }, 0.5, 1.5));

    auto composite = std::make_shared<CompositeShape>();
    composite->add(std::make_shared<Rectangle>(Point{ 1.0, 1.0 }, Point{ 3.0, 4.0 }));
    composite->add(std::make_shared<Ring>(Point{ 5.0, 2.0 }, 0.5, 1.5));

    shapes.push_back(composite);

    std::cout << "Before scale:\n";
    for (const auto& shape : shapes)
    {
        if (shape->getName() == "COMPOSITE")
        {
            printComposite(dynamic_cast<const CompositeShape&>(*shape));
        }
        else
        {
            printShape(*shape);
        }
        std::cout << '\n';
    }

    for (const auto& shape : shapes)
    {
        shape->scale(2.0);
    }

    std::cout << "\nAfter scale:\n";
    for (const auto& shape : shapes)
    {
        if (shape->getName() == "COMPOSITE")
        {
            printComposite(dynamic_cast<const CompositeShape&>(*shape));
        }
        else
        {
            printShape(*shape);
        }
        std::cout << '\n';
    }

    return 0;
}
