#ifndef TRAPEZOID_H
#define TRAPEZOID_H
#include "shape.h"
class Trapezoid : public Shape
{
   private:

   Point left_;
   float bottom_;
   float top_;
   float height_;

   public:

   Trapezoid(const Point& left, const float& bottom, const float& top, const float& height):
   left_(left),
   bottom_(bottom),
   top_(top),
   height_(height)
   {}

   float getArea() const override ;
   Point getCentre() const override ;
   void move(const float& movex, const float& movey) override ;
   void scale(const float& k) override ;
   std::string getName() const override ;
   void getBox(float &xmin, float &ymin, float  &xmax, float &ymax) const override;
};
#endif

