#include "Commands.h"
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <cmath>
static double get_area(const Polygon& poly)
{
    const auto& pts = poly.points;

    return std::abs(std::inner_product(
        pts.begin(), std::prev(pts.end()),
        std::next(pts.begin()),
        0.0,
        std::plus<>(),
        [](const Point& a, const Point& b) {
            return a.x * b.y - a.y * b.x;
        }
    ) + (pts.back().x * pts.front().y - pts.back().y * pts.front().x)) / 2.0;
}
static std::size_t get_size(const Polygon& p)
{
    return p.points.size();
}
static std::vector<Point> normalize(const Polygon& poly)
{
    std::vector<Point> pts = poly.points;

    // Сортируем точки, чтобы порядок не влиял
    std::sort(pts.begin(), pts.end());

    // Берём минимальную точку как базовую
    Point base = pts.front();

    // Переносим фигуру в начало координат
    std::transform(
        pts.begin(),
        pts.end(),
        pts.begin(),
        [base](const Point& p)
        {
            return Point{
                p.x - base.x,
                p.y - base.y
            };
        }
    );

    return pts;
}
static bool is_same(const Polygon& a, const Polygon& b)
{
    if (a.points.size() != b.points.size()) {
        return false;
    }
    return normalize(a) == normalize(b);
}
void area(std::vector<Polygon> polygons, std::string param, std::ostream& out)
{
    if (param == "EVEN") {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p) {
                return (p.points.size() % 2 == 0) ? acc + get_area(p) : acc;
            });
        out << sum << "\n";
    }
    else if (param == "ODD") {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p) {
                return (p.points.size() % 2 != 0) ? acc + get_area(p) : acc;
            });
        out << sum << "\n";
    }
    else if (param == "MEAN") {
        if (polygons.empty()) throw std::logic_error("<INVALID COMMAND>");
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p) {
                return acc + get_area(p);
            });
        out << sum / polygons.size() << "\n";
    }
    else {
        int n = std::stoi(param);
        if (n < 3) throw std::logic_error("<INVALID COMMAND>");

        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [n](double acc, const Polygon& p) {
                return (p.points.size() == n) ? acc + get_area(p) : acc;
            });

        out << sum << "\n";
    }
}
void max(std::vector<Polygon> polygons, std::string param, std::ostream& out)
{
    if (polygons.empty()) throw std::logic_error("<INVALID COMMAND>");

    if (param == "AREA") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return get_area(a) < get_area(b);
            });
        out << get_area(*it) << "\n";
    }
    else if (param == "VERTEXES") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << it->points.size() << "\n";
    }
    else throw std::logic_error("<INVALID COMMAND>");
}
void min(std::vector<Polygon> polygons, std::string param, std::ostream& out)
{
    if (polygons.empty()) throw std::logic_error("<INVALID COMMAND>");

    if (param == "AREA") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return get_area(a) < get_area(b);
            });
        out << get_area(*it) << "\n";
    }
    else if (param == "VERTEXES") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << it->points.size() << "\n";
    }
    else throw std::logic_error("<INVALID COMMAND>");
}
void ccount(std::vector<Polygon> polygons, std::string param, std::ostream& out)
{
    if (param == "EVEN") {
        out << std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p) {
                return p.points.size() % 2 == 0;
            }) << "\n";
    }
    else if (param == "ODD") {
        out << std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p) {
                return p.points.size() % 2 != 0;
            }) << "\n";
    }
    else {
        int n = std::stoi(param);
        if (n < 3) throw std::logic_error("<INVALID COMMAND>");

        out << std::count_if(polygons.begin(), polygons.end(),
            [n](const Polygon& p) {
                return p.points.size() == n;
            }) << "\n";
    }
}
void rmecho(std::vector<Polygon>& polygons, const Polygon& polygon, std::ostream& out)
{
    Polygon previous;
    auto end = std::remove_if(polygons.begin(), polygons.end(),
        [&](Polygon c) {
            if (previous == c && c == polygon) {
                return true;
            }
            else {
                previous = c;
                return false;
            }
        });
    out << std::distance(end, polygons.end()) << "\n";
}
void same(std::vector<Polygon> polygons,
    const Polygon& polygon,
    std::ostream& out)
{
    out << std::count_if(
        polygons.begin(),
        polygons.end(),
        [&polygon](const Polygon& p)
        {
            return is_same(p, polygon);
        }
    ) << "\n";
}
