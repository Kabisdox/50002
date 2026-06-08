#ifndef POINT_H
#define POINT_H

struct Point {
    double x = 0;
    double y = 0;

    Point() = default;
    Point(const double x, const double y):
        x(x),
        y(y)
    {}
};

#endif //POINT_H
