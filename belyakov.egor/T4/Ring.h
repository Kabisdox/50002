#ifndef Ring_H
#define Ring_H

#include "shape.h"
#include <cmath>

class Ring : public Shape
{
private:
    double radius1_;
    double radius2_;
    Point centre_;

public:
    Ring() = delete;
    ~Ring() override = default;
    Ring(const Ring& other) = default;
    Ring& operator=(const Ring& other) = default;

    Ring(Point center, double radius1, double radius2);

    double getArea() const override;
    Point getCenter() const override;
    void move(double deltaX, double deltaY) override;
    void scale(double factor) override;
    std::string getName() const override;

    double getRadius1() const;
    double getRadius2() const;

    double getLeft() const;
    double getRight() const ;
    double getBottom() const ;
    double getTop() const ;


};
#endif // Ring

