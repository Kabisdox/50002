#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

#include "rectangle.h"
#include "circle.h"
#include "compositeshape.h"

void printShape(const std::shared_ptr<Shape>& shape, int level = 0)
{
    for (int i = 0; i < level; ++i)
    {
        std::cout << "  ";
    }

    Point center = shape->getCenter();

    std::cout << "["
        << shape->getName()
        << ", ("
        << center.x
        << ", "
        << center.y
        << "), "
        << shape->getArea()
        << "]"
        << std::endl;

    CompositeShape* composite = dynamic_cast<CompositeShape*>(shape.get());

    if (composite != nullptr)
    {
        const std::vector<std::shared_ptr<Shape>>& shapes = composite->getShapes();

        for (size_t i = 0; i < shapes.size(); ++i)
        {
            printShape(shapes[i], level + 1);
        }
    }
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);

    std::vector<std::shared_ptr<Shape>> shapes;

    shapes.push_back(std::make_shared<Rectangle>(Point{ 0.0, 0.0 }, Point{ 4.0, 3.0 }));
    shapes.push_back(std::make_shared<Rectangle>(Point{ 2.0, 1.0 }, Point{ 6.0, 5.0 }));
    shapes.push_back(std::make_shared<Circle>(Point{ 1.0, 1.0 }, 2.0));
    shapes.push_back(std::make_shared<Circle>(Point{ 5.0, 2.0 }, 1.5));

    std::shared_ptr<CompositeShape> composite = std::make_shared<CompositeShape>();
    composite->addShape(std::make_shared<Rectangle>(Point{ 0.0, 0.0 }, Point{ 2.0, 2.0 }));
    composite->addShape(std::make_shared<Circle>(Point{ 4.0, 4.0 }, 1.0));

    shapes.push_back(composite);

    std::cout << "Before scale:" << std::endl;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
        printShape(shapes[i]);
    }

    for (size_t i = 0; i < shapes.size(); ++i)
    {
        shapes[i]->scale(2.0);
    }

    std::cout << std::endl;
    std::cout << "After scale:" << std::endl;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
        printShape(shapes[i]);
    }

    return 0;
}
