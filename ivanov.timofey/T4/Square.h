#ifndef SQUARE_H
#define SQUARE_H
#include "Shape.h"

class Square : public Shape {
private:
    Point bottomLeft_;
    double side_;

public:
    Square(const Point& bottomLeft, double side);
    ~Square() override = default;
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    void getBoundingBox(Point& bottomLeft, Point& topRight) const override;
    void print() const override;
};

#endif
