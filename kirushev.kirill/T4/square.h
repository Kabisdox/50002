#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"
#include "point.h"

class Square : public Shape {
public:
    Square(const Point& bottomLeft, const std::size_t sideLength):
        bottomLeft_(bottomLeft),
        sideLength_(sideLength)
    {}
    ~Square() override = default;

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double k) override;

    std::string getName() const override;

private:
    Point bottomLeft_;
    std::size_t sideLength_ = 0;
};

#endif //SQUARE_H
