#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include <cmath>

class Rectangle : public Shape
{
private:
    double width_;
    double height_;
    Point centre_;

public:
    Rectangle() = delete;
    ~Rectangle() override = default;
    Rectangle(const Rectangle& other) = default;
    Rectangle& operator=(const Rectangle& other) = default;

    Rectangle(Point left, Point right);

    double getArea() const override;
    Point getCenter() const override;
    void move(double deltaX, double deltaY) override;
    void scale(double factor) override;
    std::string getName() const override;

    double getWidth() const;
    double getHeight() const;

    double getLeft() const ;
    double getRight() const ;
    double getBottom() const ;
    double getTop() const ;
};

#endif
