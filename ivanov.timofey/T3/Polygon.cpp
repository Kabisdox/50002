#include "Polygon.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <sstream>
#include <string>

std::istream& operator>>(std::istream& in, Point& point)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    char c1, c2, c3;
    if (!(in >> c1 && c1 == '('))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!(in >> point.x))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!(in >> c2 && c2 == ';'))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!(in >> point.y))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!(in >> c3 && c3 == ')'))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out << "(" << point.x << ";" << point.y << ")";
    return out;
}

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    size_t vertexCount;
    if (!(in >> vertexCount))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (vertexCount < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    Polygon temp;
    for (size_t i = 0; i < vertexCount; ++i)
    {
        Point p;
        if (!(in >> p))
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        temp.points.push_back(p);
    }

    if (temp.points.size() != vertexCount)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    polygon = temp;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Polygon& polygon)
{
    out << polygon.points.size();
    for (const auto& point : polygon.points)
    {
        out << " " << point;
    }
    return out;
}

double Polygon::area() const
{
    if (points.size() < 3) return 0.0;

    double sum = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % points.size()];
        sum += static_cast<double>(p1.x) * p2.y - static_cast<double>(p2.x) * p1.y;
    }
    return std::fabs(sum) / 2.0;
}

bool Polygon::hasRightAngle() const
{
    if (points.size() < 3) return false;

    for (size_t i = 0; i < points.size(); ++i)
    {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % points.size()];
        const Point& p3 = points[(i + 2) % points.size()];

        int dx1 = p2.x - p1.x;
        int dy1 = p2.y - p1.y;
        int dx2 = p3.x - p2.x;
        int dy2 = p3.y - p2.y;

        if (dx1 * dx2 + dy1 * dy2 == 0)
        {
            return true;
        }
    }
    return false;
}

bool Polygon::isPermutationOf(const Polygon& other) const
{
    if (points.size() != other.points.size()) return false;

    std::vector<Point> sortedThis = points;
    std::vector<Point> sortedOther = other.points;

    std::sort(sortedThis.begin(), sortedThis.end());
    std::sort(sortedOther.begin(), sortedOther.end());

    return sortedThis == sortedOther;
}

void printPolygons(const std::vector<Polygon>& polygons)
{
    for (const auto& polygon : polygons)
    {
        std::cout << polygon << std::endl;
    }
}

std::vector<std::size_t> getIndexes(std::size_t n)
{
    std::vector<std::size_t> indexes(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        indexes[i] = i;
    }
    return indexes;
}

long long cross(const Point& a, const Point& b, const Point& c)
{
    return static_cast<long long>(b.x - a.x) * (c.y - a.y)
        - static_cast<long long>(b.y - a.y) * (c.x - a.x);
}

bool onSegment(const Point& p, const Point& q, const Point& r)
{
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}

bool segmentsIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
    long long o1 = cross(p1, q1, p2);
    long long o2 = cross(p1, q1, q2);
    long long o3 = cross(p2, q2, p1);
    long long o4 = cross(p2, q2, q1);

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return (o1 > 0) != (o2 > 0) && (o3 > 0) != (o4 > 0);
}

bool pointInPolygon(const Point& point, const Polygon& polygon)
{
    if (polygon.points.size() < 3) return false;

    int count = 0;
    for (size_t i = 0; i < polygon.points.size(); ++i)
    {
        const Point& p1 = polygon.points[i];
        const Point& p2 = polygon.points[(i + 1) % polygon.points.size()];

        if ((p1.y > point.y) != (p2.y > point.y))
        {
            double xIntersect = static_cast<double>(p1.x)
                + static_cast<double>(point.y - p1.y)
                * static_cast<double>(p2.x - p1.x)
                / static_cast<double>(p2.y - p1.y);

            if (point.x < xIntersect)
            {
                ++count;
            }
        }
    }
    return count % 2 == 1;
}

bool polygonsIntersect(const Polygon& a, const Polygon& b)
{
    for (size_t i = 0; i < a.points.size(); ++i)
    {
        if (pointInPolygon(a.points[i], b)) return true;
    }
    for (size_t i = 0; i < b.points.size(); ++i)
    {
        if (pointInPolygon(b.points[i], a)) return true;
    }
    for (size_t i = 0; i < a.points.size(); ++i)
    {
        const Point& a1 = a.points[i];
        const Point& a2 = a.points[(i + 1) % a.points.size()];
        for (size_t j = 0; j < b.points.size(); ++j)
        {
            const Point& b1 = b.points[j];
            const Point& b2 = b.points[(j + 1) % b.points.size()];
            if (segmentsIntersect(a1, a2, b1, b2)) return true;
        }
    }
    return false;
}
