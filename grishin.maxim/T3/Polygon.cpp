#include <iostream>
#include <algorithm>
#include <iterator>
#include "Polygon.h"
std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    size_t numberDots;
    in >> numberDots;
    if (!in || numberDots < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }
    polygon.points.clear();
    try {
        std::generate_n(
            std::back_inserter(polygon.points),
            numberDots,
            [&in]()
            {
                Point p;
                if (!(in >> p)) {
                    throw std::ios_base::failure("Point read error");
                }
                return p;
            }
        );
    }
    catch (...) {
        polygon.points.clear();
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::istream& operator>>(std::istream& in, Delimeter&& chr) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c;
    in >> c;
    if (!in || (c != chr.exp)) {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    return in;
}
std::istream& operator>>(std::istream& in, Point& point) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    in >> Delimeter{ '(' } >> point.x >> Delimeter{ ';' } >> point.y >> Delimeter{ ')' };
    if (!in) {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    return in;
}
