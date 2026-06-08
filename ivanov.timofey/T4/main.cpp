#include <iostream>
#include <memory>
#include <iomanip>
#include "Rectangle.h"
#include "Square.h"
#include "CompositeShape.h"

void testRectangle()
{
    std::cout << "Rectangle==================" << std::endl;

    try {
        Rectangle rect(Point(1.0, 1.0), Point(5.0, 4.0));

        rect.print();
        std::cout << std::endl;

        std::cout << "Area: " << rect.getArea() << std::endl;
        Point center = rect.getCenter();
        std::cout << "Centre: (" << center.x_ << ", " << center.y_ << ")" << std::endl;

        std::cout << "move on (2, 1):" << std::endl;
        rect.move(2.0, 1.0);
        rect.print();
        std::cout << std::endl;

        std::cout << "make bigger in 2 times:" << std::endl;
        rect.scale(2.0);
        rect.print();
        std::cout << std::endl;

        Point bottomLeft, topRight;
        rect.getBoundingBox(bottomLeft, topRight);
        std::cout << "BoundingBox: "
            << "(" << bottomLeft.x_ << ", " << bottomLeft.y_ << ") - "
            << "(" << topRight.x_ << ", " << topRight.y_ << ")" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testSquare()
{
    std::cout << "Square===============" << std::endl;

    try {
        Square square(Point(2.0, 2.0), 3.0);

        square.print();
        std::cout << std::endl;

        std::cout << "Area: " << square.getArea() << std::endl;
        Point center = square.getCenter();
        std::cout << "Centre: (" << center.x_ << ", " << center.y_ << ")" << std::endl;

        std::cout << "Move on (1, -1):" << std::endl;
        square.move(1.0, -1.0);
        square.print();
        std::cout << std::endl;

        std::cout << "Make bigger in 2 times:" << std::endl;
        square.scale(1.5);
        square.print();
        std::cout << std::endl;

        Point bottomLeft, topRight;
        square.getBoundingBox(bottomLeft, topRight);
        std::cout << "BoundingBox: "
            << "(" << bottomLeft.x_ << ", " << bottomLeft.y_ << ") - "
            << "(" << topRight.x_ << ", " << topRight.y_ << ")" << std::endl;

        try {
            Square invalidSquare(Point(0, 0), -5.0);
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testCompositeShape()
{
    std::cout << "CompositeShape=============================" << std::endl;

    try {
        CompositeShape scene;

        std::unique_ptr<Shape> square1(new Square(Point(0.0, 0.0), 2.0));
        scene.addShape(std::move(square1));

        std::unique_ptr<Shape> rect(new Rectangle(Point(3.0, 1.0), Point(5.0, 4.0)));
        scene.addShape(std::move(rect));

        std::unique_ptr<Shape> square2(new Square(Point(1.0, 3.0), 1.5));
        scene.addShape(std::move(square2));

        std::cout << "Composite shape:" << std::endl;
        scene.print();

        std::cout << "Number of shapes: " << scene.getShapeCount() << std::endl;
        std::cout << "Area: " << scene.getArea() << std::endl;

        Point center = scene.getCenter();
        std::cout << "Centre: (" << center.x_ << ", " << center.y_ << ")" << std::endl;

        std::cout << "move on (1, 1):" << std::endl;
        scene.move(1.0, 1.0);
        scene.print();

        std::cout << "Make bigger in 2 times:" << std::endl;
        scene.scale(2.0);
        scene.print();

        Point bottomLeft, topRight;
        scene.getBoundingBox(bottomLeft, topRight);
        std::cout << "BoundingBox: "
            << "(" << bottomLeft.x_ << ", " << bottomLeft.y_ << ") - "
            << "(" << topRight.x_ << ", " << topRight.y_ << ")" << std::endl;

        try {
            CompositeShape empty;
            empty.scale(2.0);
        }
        catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        try {
            CompositeShape empty;
            empty.getCenter();
            std::cout << "Empty centr(0,0)" << std::endl;
        }
        catch (...) {
            std::cout << "Error" << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testPolymorphism()
{

    try {
        std::vector<std::unique_ptr<Shape>> shapes;

        shapes.push_back(std::unique_ptr<Shape>(new Rectangle(Point(0, 0), Point(4, 3))));
        shapes.push_back(std::unique_ptr<Shape>(new Square(Point(1, 1), 2)));

        for (const auto& shape : shapes) {
            std::cout << "  ";
            shape->print();
            std::cout << shape->getArea() << std::endl;
        }

        std::cout << "Bigger in 2 times:" << std::endl;
        for (auto& shape : shapes) {
            shape->scale(2.0);
        }

        for (const auto& shape : shapes) {
            std::cout << "  ";
            shape->print();
            std::cout << shape->getArea() << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);
    testRectangle();
    std::cout << "========================================" << std::endl;
    testSquare();
    std::cout << "========================================" << std::endl;
    testCompositeShape();
    std::cout << "========================================" << std::endl;
    testPolymorphism();


    return 0;
}
