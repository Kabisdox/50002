#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H
#include <vector>
#include <memory>
#include <iomanip>
#include "rectangle.h"
#include "square.h"
class CompositeShape: public Shape {
public:
    CompositeShape() = default;
    CompositeShape(CompositeShape& other) = delete;
    CompositeShape(CompositeShape&& other) = default;
    CompositeShape& operator=(CompositeShape& other) = delete;
    CompositeShape& operator=(CompositeShape&& other) = default;

    void addShape(std::unique_ptr<Shape> shape);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    std::string getName() const override;
    void print() const override;
    void printRaw() const override;
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};
#endif // !COMPOSITESHAPE_H
