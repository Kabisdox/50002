#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "shape.h"
#include <vector>
#include <memory>

class CompositeShape : public Shape
{

    public:
    CompositeShape() = default;
    void add(std::unique_ptr<Shape> shape);
    float getArea() const override;
    Point getCentre() const override;
    void move(const float& movex, const float& movey) override;
    void scale(const float& k) override;
    std::string getName() const override;
    void getBox(float &xmin, float &ymin, float &xmax, float &ymax) const override;

    const std::vector<std::unique_ptr<Shape>>& getShapes() const
    {
        return shapes_;
    }

    private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};
#endif

