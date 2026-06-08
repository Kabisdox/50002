#ifndef SQUARE_H
#define SQUARE_H
#include "point.h"
#include "shape.h"
#include <iomanip>
#include <string>
class Square: public Shape {
public:
    Square();
    Square(Point top, double lenght);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    std::string getName() const override;
    void print() const override;
    void printRaw() const override;
private:
    Point top_;
    double lenght_;
};
#endif // !SQUARE_H
