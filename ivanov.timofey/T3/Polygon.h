#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iosfwd>

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

std::istream& operator>>(std::istream&, Point&);
std::ostream& operator<<(std::ostream&, const Point&);

struct Polygon {
    std::vector<Point> points;

    bool operator==(const Polygon& other) const {
        return points == other.points;
    }

    double area() const;
    bool hasRightAngle() const;

    bool isPermutationOf(const Polygon& other) const;
};

std::istream& operator>>(std::istream&, Polygon&);
std::ostream& operator<<(std::ostream&, const Polygon&);

void printPolygons(const std::vector<Polygon>& polygons);
std::vector<std::size_t> getIndexes(std::size_t);
bool pointInPolygon(const Point&, const Polygon&);
long long cross(const Point&, const Point&, const Point&);
bool onSegment(const Point&, const Point&, const Point&);
bool segmentsIntersect(const Point&, const Point&, const Point&, const Point&);
bool polygonsIntersect(const Polygon&, const Polygon&);

#endif
