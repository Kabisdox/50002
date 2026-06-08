#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.h"
#include <iomanip>
#include "point.h"
#include <string>
class Rectangle : public Shape
{
public:
    Rectangle();
    Rectangle(Point pointA, Point pointB);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    std::string getName() const override;
    void print() const override;
    void printRaw() const override;
private:
    Point pointA_;
    Point pointB_;
};
#endif // !RECTANGLE_H
