#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "point.h"

class Rectangle : public Shape {
public:
    Rectangle(const Point& bottomLeft, const Point& topRight):
        bottomLeft_(bottomLeft),
        topRight_(topRight)
    {}
    ~Rectangle() override = default;

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double k) override;

    std::string getName() const override;

private:
    Point bottomLeft_;
    Point topRight_;
};

#endif //RECTANGLE_H
