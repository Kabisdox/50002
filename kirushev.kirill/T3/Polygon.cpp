#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>

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

std::vector<std::size_t> getIndexes(const std::size_t size) {
    std::vector<std::size_t> indexes(size);
    std::iota(indexes.begin(), indexes.end(), 0);
    return indexes;
}

bool pointInPolygon(const Point& point, const Polygon& polygon) {
    const std::vector<std::size_t> indexes = getIndexes(polygon.points.size());

    const bool onBorder = std::any_of(
        indexes.begin(),
        indexes.end(),
        [&point, &polygon](const std::size_t i) {
            const Point& a = polygon.points[i];
            const Point& b = polygon.points[(i + 1) % polygon.points.size()];

            return onSegment(point, a, b);
        }
    );

    if (onBorder) {
        return true;
    }

    return std::accumulate(
        indexes.begin(),
        indexes.end(),
        false,
        [&point, &polygon](const bool inside, const std::size_t i) {
            const Point& a = polygon.points[i];
            const Point& b = polygon.points[(i + 1) % polygon.points.size()];

            const bool crosses =
                ((a.y > point.y) != (b.y > point.y)) &&
                (
                    point.x < static_cast<double>(b.x - a.x)
                    * static_cast<double>(point.y - a.y)
                    / static_cast<double>(b.y - a.y)
                    + a.x
                );

            return crosses ? !inside : inside;
        }
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
    const std::vector<std::size_t> lhsIndexes = getIndexes(lhs.points.size());
    const std::vector<std::size_t> rhsIndexes = getIndexes(rhs.points.size());

    const bool edgesIntersect = std::any_of(
        lhsIndexes.begin(),
        lhsIndexes.end(),
        [&lhs, &rhs, &rhsIndexes](const std::size_t i) {
            const Point& a = lhs.points[i];
            const Point& b = lhs.points[(i + 1) % lhs.points.size()];

            return std::any_of(
                rhsIndexes.begin(),
                rhsIndexes.end(),
                [&rhs, &a, &b](const std::size_t j) {
                    const Point& c = rhs.points[j];
                    const Point& d = rhs.points[(j + 1) % rhs.points.size()];

                    return segmentsIntersect(a, b, c, d);
                }
            );
        }
    );

    return edgesIntersect
        || pointInPolygon(lhs.points.front(), rhs)
        || pointInPolygon(rhs.points.front(), lhs);
}
