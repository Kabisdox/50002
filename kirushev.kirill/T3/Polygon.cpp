#include <iostream>
#include <algorithm>
#include <iterator>

#include "Polygon.h"
#include "Structs.h"

// Point
std::istream& operator>>(std::istream& is, Point& point) {
    const std::istream::sentry sentry(is);

    if (!sentry) {
        return is;
    }

    is >> DelimiterIO{'('} >> point.x >> DelimiterIO{';'} >> point.y >> DelimiterIO{')'};
    return is;
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    const std::ostream::sentry sentry(os);

    if (!sentry) {
        return os;
    }

    os << "(" << point.x << ";" << point.y << ")";
    return os;
}

const Point& getNextPoint(const Polygon& polygon, const Point& point) {
    const auto index = &point - polygon.points.data();
    return polygon.points[(index + 1) % polygon.points.size()];
}

// Polygon
std::istream& operator>>(std::istream& is, Polygon& polygon) {
    const std::istream::sentry sentry(is);

    if (!sentry) {
        return is;
    }

    std::size_t count = 0;
    if (!(is >> count) || count < 3) {
        is.setstate(std::ios::failbit);
        return is;
    }

    is >> std::noskipws;
    Polygon inputPolygon;
    inputPolygon.points.reserve(count);

    std::generate_n(
        std::back_inserter(inputPolygon.points),
        count,
        [&is]() {
            Point point;
            char c = '\0';

            if (!(is.get(c) && c == ' ' && is >> point)) {
                is.setstate(std::ios::failbit);
            }
            return point;
        }
        );

    char checkEnd = '\0';
    if (is.get(checkEnd)) {
        if (checkEnd != '\n') {
            is.setstate(std::ios::failbit);
        }
    } else {
        if (is.eof()) {
            is.clear();
        } else {
            is.setstate(std::ios::failbit);
        }
    }

    is >> std::skipws;

    if (is) {
        polygon = std::move(inputPolygon);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Polygon& polygon) {
    os << polygon.points.size();
    std::copy(
        polygon.points.begin(),
        polygon.points.end(),
        std::ostream_iterator<Point>(os, " ")
        );

    return os;
}

void printPolygons(const std::vector<Polygon>& polygons) {
    std::cout << "Polygon size: " << polygons.size() << '\n';

    std::copy(
        polygons.begin(),
        polygons.end(),
        std::ostream_iterator<Polygon>(std::cout, "\n")
    );
}

long long cross(const Point& a, const Point& b, const Point& c) {
    return static_cast<long long>(b.x - a.x) * (c.y - a.y)
         - static_cast<long long>(b.y - a.y) * (c.x - a.x);
}

bool onSegment(const Point& p, const Point& a, const Point& b) {
    return cross(a, b, p) == 0
        && std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x)
        && std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

bool segmentsIntersect(const Point& a, const Point& b, const Point& c, const Point& d) {
    const long long ab_c = cross(a, b, c);
    const long long ab_d = cross(a, b, d);
    const long long cd_a = cross(c, d, a);
    const long long cd_b = cross(c, d, b);

    const bool differentSides1 = ((ab_c > 0) && (ab_d < 0)) || ((ab_c < 0) && (ab_d > 0));
    const bool differentSides2 = ((cd_a > 0) && (cd_b < 0)) || ((cd_a < 0) && (cd_b > 0));

    return onSegment(c, a, b)
        || onSegment(d, a, b)
        || onSegment(a, c, d)
        || onSegment(b, c, d)
        || (differentSides1 && differentSides2);
}

bool polygonsIntersect(const Polygon& lhs, const Polygon& rhs) {
    return std::any_of(
        lhs.points.begin(),
        lhs.points.end(),
        [&lhs, &rhs](const Point& a) {
            const Point& b = getNextPoint(lhs, a);

            return std::any_of(
                rhs.points.begin(),
                rhs.points.end(),
                [&rhs, &a, &b](const Point& c) {
                    const Point& d = getNextPoint(rhs, c);
                    return segmentsIntersect(a, b, c, d);
                }
            );
        }
    );
}
