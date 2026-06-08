#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <string>
#include <iostream>
struct StrictDelim {
    char exp;
};

class iofmtguard {
public:
    iofmtguard(std::basic_ios<char>& s) noexcept;
    ~iofmtguard();

private:
    std::basic_ios<char>& s_;
    char fill_;
    std::streamsize precision_;
    std::ios::fmtflags fmt_;
};

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Point& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

std::istream& operator>>(std::istream& in, Point& point);
std::ostream& operator<<(std::ostream& out, const Point& point);

struct Polygon {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const {
        return points == other.points;
    }
};

std::istream& operator>>(std::istream& in, Polygon& polygon);
#endif // !POLYGON_H
