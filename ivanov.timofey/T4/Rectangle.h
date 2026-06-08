#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Shape.h"

class Rectangle : public Shape {
private:
    Point bottomLeft_;
    Point upperRight_;
public:
    Rectangle(const Point& bottomLeft, const Point& upperRight);
    ~Rectangle() override = default;
    double getArea() const override;
    Point getCenter() const override;
    void move(double x, double y) override;
    void scale(double k) override;
    std::string getName() const override;
    void getBoundingBox(Point& bottomLeft, Point& topRight) const override;
    void print() const override;
};

#endif
