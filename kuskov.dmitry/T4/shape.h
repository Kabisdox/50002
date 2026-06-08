#ifndef SHAPE_H
#define SHAPE_H
#include <string>
#include "Point.h"
class Shape
{
    public:
    virtual float getArea() const = 0;
    virtual Point getCentre() const = 0;
    virtual void move(const float& movex, const float& movey) = 0;
    virtual void scale(const float& k) = 0;
    virtual std::string getName() const = 0;
    virtual void getBox(float &xmin, float &ymin, float &xmax, float &ymax) const = 0;
    virtual ~Shape()
    {}
};
#endif

