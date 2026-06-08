#ifndef POINT_H
#define POINT_H
#include <type_traits>
#include <utility>
#include <iostream>
struct Point
{
    float x_;
    float y_;

    Point(const float x, const float y):
    x_(x),
    y_(y)
    {}

    void moveX(const float& movex)
    {
        x_ += movex;
    }

    void moveY(const float& movey)
    {
        y_ += movey;
    }

    void move(const float& movex, const float& movey)
    {
        moveX(movex);
        moveY(movey);
    }
};
#endif

