#include "CompositeShape.h"
#include <stdexcept>

void CompositeShape::add(const std::shared_ptr<Shape>& shape)
{
    if (!shape)
    {
        throw std::invalid_argument("Null shape");
    }

    if (shape->getName() == "COMPOSITE")
    {
        throw std::invalid_argument("Composite cannot contain composite");
    }

    shapes_.push_back(shape);
}

double CompositeShape::getArea() const
{
    double area = 0.0;

    for (const auto& shape : shapes_)
    {
        area += shape->getArea();
    }

    return area;
}

Point CompositeShape::getCenter() const
{
    if (shapes_.empty())
    {
        throw std::logic_error("Composite is empty");
    }

    Point center;
    center.x_ = (getLeft() + getRight()) / 2.0;
    center.y_ = (getBottom() + getTop()) / 2.0;

    return center;
}

void CompositeShape::move(double deltaX, double deltaY)
{
    for (const auto& shape : shapes_)
    {
        shape->move(deltaX, deltaY);
    }
}

void CompositeShape::scale(double factor)
{
    if (factor <= 0)
    {
        throw std::invalid_argument("Scale factor must be positive");
    }

    Point commonCenter = getCenter();

    for (const auto& shape : shapes_)
    {
        Point shapeCenter = shape->getCenter();

        double dx = shapeCenter.x_ - commonCenter.x_;
        double dy = shapeCenter.y_ - commonCenter.y_;

        shape->move(dx * (factor - 1.0), dy * (factor - 1.0));
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const
{
    return "COMPOSITE";
}

double CompositeShape::getLeft() const
{
    if (shapes_.empty())
    {
        throw std::logic_error("Composite is empty");
    }

    double left = shapes_[0]->getLeft();

    for (size_t i = 1; i < shapes_.size(); ++i)
    {
        if (shapes_[i]->getLeft() < left)
        {
            left = shapes_[i]->getLeft();
        }
    }

    return left;
}

double CompositeShape::getRight() const
{
    if (shapes_.empty())
    {
        throw std::logic_error("Composite is empty");
    }

    double right = shapes_[0]->getRight();

    for (size_t i = 1; i < shapes_.size(); ++i)
    {
        if (shapes_[i]->getRight() > right)
        {
            right = shapes_[i]->getRight();
        }
    }

    return right;
}

double CompositeShape::getBottom() const
{
    if (shapes_.empty())
    {
        throw std::logic_error("Composite is empty");
    }

    double bottom = shapes_[0]->getBottom();

    for (size_t i = 1; i < shapes_.size(); ++i)
    {
        if (shapes_[i]->getBottom() < bottom)
        {
            bottom = shapes_[i]->getBottom();
        }
    }

    return bottom;
}

double CompositeShape::getTop() const
{
    if (shapes_.empty())
    {
        throw std::logic_error("Composite is empty");
    }

    double top = shapes_[0]->getTop();

    for (size_t i = 1; i < shapes_.size(); ++i)
    {
        if (shapes_[i]->getTop() > top)
        {
            top = shapes_[i]->getTop();
        }
    }

    return top;
}

const std::vector<std::shared_ptr<Shape>>& CompositeShape::getShapes() const
{
    return shapes_;
}
