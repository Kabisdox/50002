#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H

#include "shape.h"
#include <vector>
#include <memory>

class CompositeShape : public Shape
{
private:
    std::vector<std::shared_ptr<Shape>> shapes_;

public:
    CompositeShape() = default;
    ~CompositeShape() override = default;

    CompositeShape(const CompositeShape& other) = default;
    CompositeShape& operator=(const CompositeShape& other) = default;

    void add(const std::shared_ptr<Shape>& shape);

    double getArea() const override;
    Point getCenter() const override;

    void move(double deltaX, double deltaY) override;
    void scale(double factor) override;

    std::string getName() const override;

    double getLeft() const override;
    double getRight() const override;
    double getBottom() const override;
    double getTop() const override;

    const std::vector<std::shared_ptr<Shape>>& getShapes() const;
};

#endif
