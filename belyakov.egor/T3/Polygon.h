#ifndef _POLYGON_H
#define _POLYGON_H
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <limits>
#include <sstream>
#include <map>
#include <algorithm>

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const
    {
        return (other.x == x) && (other.y == y);
    }
};

struct Polygon {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const
    {
        if (points.size() != other.points.size()) {
            return false;
        }
        return std::equal(points.begin(), points.end(), other.points.begin());
    }
};

struct DelimiterIO {
    char del;
};

class iofmtguard {
public:
    iofmtguard(std::basic_ios<char>& s) noexcept;
    ~iofmtguard();
private:
    std::basic_ios<char>& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, Point& dest);
std::istream& operator>>(std::istream& in, Polygon& dest);
#endif