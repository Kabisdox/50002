#ifndef ENVY_HPP
#define ENVY_HPP
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <functional>
#include <numeric>
#include <algorithm>
#include <cmath>

struct Point
{
    int x;
    int y;

    bool equal(const Point p) const
    {
        if (p.x - x == p.y - y)
            return true;
        return false;
    }

    bool equalN(const Point p, const int n) const
    {
        if (p.x - x == p.y - y && p.x - x == n)
            return true;
        return false;
    }
};

struct Polygon
{
    std::vector<Point> points;
    float centreX()
    {
        return (std::accumulate(points.begin(), points.end(), 0, [=](double acc, Point p){return (acc + p.x);})) / points.size();
    }

    float centreY()
    {
        return (std::accumulate(points.begin(), points.end(), 0, [=](double acc, Point p){return (acc + p.y);})) / points.size();
    }
};

bool comp(const Point& a, const Point& b, float x, float y)
{
    bool app = (a.y > y);
    bool bpp = (b.y > y);
    if (app != bpp)
        return app;
    float cross = (a.x - x) *  (b.y - y) - (a.y - y) * (b.x - x);
    return cross > 0;
}

bool comp2(const Point& p, const Point& pp)
{
    return p.x < pp.x || (p.x == pp.x && p.y < pp.y);
}

float area(Polygon poly)
{
    if (poly.points.size() < 3)
        return 0.0;
    float cx = poly.centreX();
    float cy = poly.centreY();
    std::sort(poly.points.begin(), poly.points.end(), [cx, cy](const Point& a, const Point& b) -> bool {return comp(a, b, cx, cy);});
    float sum = std::accumulate(poly.points.begin(), poly.points.end(), 0.0,
            [&poly, i = 0](float acc, const Point& p) mutable
            {
                const Point& next = poly.points[(i + 1) % poly.points.size()];
                float term = p.x * next.y - next.x * p.y;
                ++i;
                return acc + term;
            });
    return static_cast<float>(std::abs(sum) * 0.5);
}

//AREA
auto AREA = [](const std::vector<Polygon>& v, std::function<double(double, const Polygon&)> parameter)
{return static_cast<float>(std::accumulate
(
    v.begin(),
    v.end(),
    0.0,
    parameter
));};

auto aEVEN = [](double acc, const Polygon& p){return acc + ((p.points.size()%2 == 0) ? area(p) : 0.0);};
auto aODD = [](double acc, const Polygon& p){return acc + ((p.points.size()%2 != 0) ? area(p) : 0.0);};
auto aMEAN = [](double acc, const Polygon& p){return acc + area(p);};

//MAX/MIN
auto MAX = [](const std::vector<Polygon>& v, std::function<bool(const Polygon&, const Polygon&)> parameter)
{return std::max_element
(
    v.begin(),
    v.end(),
    parameter
);};

auto MIN = [](const std::vector<Polygon>& v, std::function<bool(const Polygon&, const Polygon&)> parameter)
{return std::min_element
(
    v.begin(),
    v.end(),
    parameter
);};

auto mAREA = [](const Polygon& p, const Polygon& pp){return area(p) < area(pp);};
auto mVERTEXES = [](const Polygon& p, const Polygon& pp){return p.points.size() < pp.points.size();};

//COUNT
auto COUNT = [](const std::vector<Polygon>& v, std::function<bool(const Polygon&)> parameter)
{return static_cast<float>(std::count_if
(
    v.begin(),
    v.end(),
    parameter
));};

auto cEVEN = [](const Polygon& p){return p.points.size()%2 == 0;};
auto cODD = [](const Polygon& p){return p.points.size()%2 != 0;};

//SAME

bool same(Polygon same11, Polygon same22)
{
    if (same11.points.size() != same22.points.size())
        return false;
    auto same1 = same11;
    auto same2 = same22;
    std::sort(same1.points.begin(), same1.points.end(), comp2);
    std::sort(same2.points.begin(), same2.points.end(), comp2);
    int n{0};
    if (!same1.points[0].equal(same2.points[0]))
        return false;
    else
        n = same1.points[0].x - same2.points[0].x;
    if (!std::equal(same1.points.begin(), same1.points.end(), same2.points.begin(),
                [&n](const Point& p, const Point& pp){return p.equalN(pp, n);}))
        return false;
    return true;
}

int dist(const Point& a, const Point& b)
{
    int x = a.x - b.x;
    int y = a.y - b.y;
    return x*x + y*y;
}

bool rect(const Polygon poly)
{
    if (poly.points.size() != 4)
        return false;
    std::vector<int> allDist(6);
    std::generate(allDist.begin(), allDist.end(), [&, i = 0, j = 1]() mutable
    {
        int d = dist(poly.points[i], poly.points[j]);
        if (++j == 4)
        {
            ++i;
            j = i + 1;
        }
        return d;
    });
    std::sort(allDist.begin(), allDist.end());
    if (allDist[0] == allDist[1] && allDist[2] == allDist[3] && allDist[4] == allDist[5])
    {
        unsigned int side1 = allDist[0];
        unsigned int side2 = allDist[2];
        unsigned int diag = allDist[4];
        return side1 + side2 == diag;
    }
    return false;
}

auto RECTS = [](const Polygon& p){return rect(p);};
#endif //ENVY_HPP
