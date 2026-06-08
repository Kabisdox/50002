#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H

#include "shape.h"
#include <vector>
#include <memory>

class CompositeShape : public Shape
{
public:
    CompositeShape();

    void addShape(std::shared_ptr<Shape> shape);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;

    const std::vector<std::shared_ptr<Shape>>& getShapes() const;

private:
    std::vector<std::shared_ptr<Shape>> shapes_;
};

#endif
