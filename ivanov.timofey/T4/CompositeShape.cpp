#include "CompositeShape.h"
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <iostream>
#include <iomanip>

void CompositeShape::addShape(std::unique_ptr<Shape> shape)
{
    if (!shape)
    {
        throw std::invalid_argument("Cannot add null shape to composite");
    }
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const
{
    double totalArea = 0.0;
    for (const auto& shape : shapes_)
    {
        totalArea += shape->getArea();
    }
    return totalArea;
}

void CompositeShape::getBoundingBox(Point& bottomLeft, Point& topRight) const
{
    if (shapes_.empty())
    {
        bottomLeft = Point(0, 0);
        topRight = Point(0, 0);
        return;
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    Point shapeBottomLeft, shapeTopRight;

    for (const auto& shape : shapes_)
    {
        shape->getBoundingBox(shapeBottomLeft, shapeTopRight);
        minX = std::min(minX, shapeBottomLeft.x_);
        minY = std::min(minY, shapeBottomLeft.y_);
        maxX = std::max(maxX, shapeTopRight.x_);
        maxY = std::max(maxY, shapeTopRight.y_);
    }

    bottomLeft = Point(minX, minY);
    topRight = Point(maxX, maxY);
}

Point CompositeShape::getCenter() const
{
    if (shapes_.empty())
    {
        return Point(0, 0);
    }

    Point bottomLeft, topRight;
    getBoundingBox(bottomLeft, topRight);

    return Point((bottomLeft.x_ + topRight.x_) / 2.0,
        (bottomLeft.y_ + topRight.y_) / 2.0);
}

void CompositeShape::move(double x, double y)
{
    for (auto& shape : shapes_)
    {
        shape->move(x, y);
    }
}

void CompositeShape::scale(double factor)
{
    if (factor <= 0)
    {
        throw std::invalid_argument("Scale factor must be positive");
    }

    if (shapes_.empty())
    {
        throw std::runtime_error("Cannot scale empty composite shape");
    }

    if (factor == 1.0)
    {
        return;
    }

    Point compositeCenter = getCenter();

    for (auto& shape : shapes_)
    {
        Point oldCenter = shape->getCenter();

        double dx = (oldCenter.x_ - compositeCenter.x_) * (factor - 1);
        double dy = (oldCenter.y_ - compositeCenter.y_) * (factor - 1);

        shape->move(dx, dy);
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const
{
    return "COMPOSITE";
}

void CompositeShape::print() const
{
    Point center = getCenter();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << getName()
        << ", (" << center.x_ << ", " << center.y_ << "), "
        << getArea() << ":" << std::endl;

    for (size_t i = 0; i < shapes_.size(); ++i)
    {
        std::cout << "  ";
        shapes_[i]->print();
        if (i < shapes_.size() - 1)
        {
            std::cout << ",";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;
}

size_t CompositeShape::getShapeCount() const
{
    return shapes_.size();
}

const Shape* CompositeShape::getShape(size_t index) const
{
    if (index >= shapes_.size())
    {
        throw std::out_of_range("Index out of range");
    }
    return shapes_[index].get();
}
