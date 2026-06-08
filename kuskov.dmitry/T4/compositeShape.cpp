#include "compositeShape.h"
#include <algorithm>
#include <limits>

void CompositeShape::add(std::unique_ptr<Shape> shape)
{
    shapes_.push_back(std::move(shape));
}

float CompositeShape::getArea() const
{
    float areaSum = 0;
    for (std::size_t i{0}; i < shapes_.size(); ++i)
    {
        areaSum += shapes_[i]->getArea();
    }
    return areaSum;
}

Point CompositeShape::getCentre() const
{
    Point res(0,0);
    if (!shapes_.empty())
    {
        float xmin;
        float ymin;
        float xmax;
        float ymax;
        getBox(xmin, ymin, xmax, ymax);
        res.move((xmin + xmax) / 2, (ymin + ymax) / 2);
    }
    return res;
}

void CompositeShape::move(const float& movex, const float& movey)
{
    for (std::size_t i{0}; i < shapes_.size(); ++i)
    {
        shapes_[i]->move(movex, movey);
    }
}

void CompositeShape::scale(const float& k)
{
    Point centre = getCentre();
    std::vector<Point> oldCentres;
    for (std::size_t i{0}; i < shapes_.size(); ++i)
    {
        oldCentres.push_back(shapes_[i]->getCentre());
    }
    for (size_t i = 0; i < shapes_.size(); ++i)
    {
        Point oldCentre = oldCentres[i];
        float dx = oldCentre.x_ - centre.x_;
        float dy = oldCentre.y_ - centre.y_;
        float newX = centre.x_ + dx * k;
        float newY = centre.y_ + dy * k;
        Point currentCentre = shapes_[i]->getCentre();
        shapes_[i]->move(newX - currentCentre.x_, newY - currentCentre.y_);
        shapes_[i]->scale(k);
    }
}

std::string  CompositeShape::getName() const
{
    return "COMPOSITE";
}

void CompositeShape::getBox(float &xmin, float &ymin, float &xmax, float &ymax) const
{
    if (shapes_.empty())
    {
        xmin = ymin = xmax = ymax = 0;
        return;
    }
    xmin = std::numeric_limits<float>::max();
    ymin = std::numeric_limits<float>::max();
    xmax = -std::numeric_limits<float>::max();
    ymax = -std::numeric_limits<float>::max();

    for (std::size_t i{0}; i < shapes_.size(); ++i)
    {
        float newxmin;
        float newymin;
        float newxmax;
        float newymax;
        shapes_[i]->getBox(newxmin, newymin, newxmax, newymax);
        xmin = std::min(xmin, newxmin);
        ymin = std::min(ymin, newymin);
        xmax = std::max(xmax, newxmax);
        ymax = std::max(ymax, newymax);
    }
}

