#include "compositeshape.h"

CompositeShape::CompositeShape()
{
}

void CompositeShape::addShape(std::shared_ptr<Shape> shape)
{
    if (shape)
    {
        shapes_.push_back(shape);
    }
}

double CompositeShape::getArea() const
{
    double sum = 0.0;

    for (size_t i = 0; i < shapes_.size(); ++i)
    {
        sum += shapes_[i]->getArea();
    }

    return sum;
}

Point CompositeShape::getCenter() const
{
    Point center;
    center.x = 0.0;
    center.y = 0.0;

    if (shapes_.empty())
    {
        return center;
    }

    for (size_t i = 0; i < shapes_.size(); ++i)
    {
        Point p = shapes_[i]->getCenter();
        center.x += p.x;
        center.y += p.y;
    }

    center.x /= shapes_.size();
    center.y /= shapes_.size();

    return center;
}

void CompositeShape::move(double dx, double dy)
{
    for (size_t i = 0; i < shapes_.size(); ++i)
    {
        shapes_[i]->move(dx, dy);
    }
}

void CompositeShape::scale(double factor)
{
    if (factor <= 0)
    {
        return;
    }

    Point center = getCenter();

    for (size_t i = 0; i < shapes_.size(); ++i)
    {
        Point shapeCenter = shapes_[i]->getCenter();

        double dx = shapeCenter.x - center.x;
        double dy = shapeCenter.y - center.y;

        shapes_[i]->move(dx * (factor - 1.0), dy * (factor - 1.0));
        shapes_[i]->scale(factor);
    }
}

std::string CompositeShape::getName() const
{
    return "COMPOSITE";
}

const std::vector<std::shared_ptr<Shape>>& CompositeShape::getShapes() const
{
    return shapes_;
}
