#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.h"
class Rectangle : public Shape
{
   private:

   Point left_;
   Point right_;

//Useful methods

   float width() const
   {
        return(right_.x_ - left_.x_);
   }

   float length() const
   {
        return(right_.y_ - left_.y_);
   }

   public:

   Rectangle(const Point& left, const Point& right):
   left_(left),
   right_(right)
   {}

   float getArea() const override;
   Point getCentre() const override;
   void move(const float& movex, const float& movey) override;
   void scale(const float& k) override;
   std::string getName() const override;
   void getBox(float &xmin, float &ymin, float &xmax, float &ymax) const override;
};
#endif

