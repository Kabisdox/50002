#ifndef RIGHT_TRAPEZOID_H
#define RIGHT_TRAPEZOID_H

#include "shape.h"

class RightTrapezoid : public Shape {
private:
    Point leftBottom;
    double bottomBase;
    double topBase;
    double height;

public:
    RightTrapezoid(Point lb, double a, double b, double h);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
};

#endif
