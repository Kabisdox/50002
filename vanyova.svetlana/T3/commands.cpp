#include <string>
#include <iomanip>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <fstream>
#include "commands.h"
#include "Polygon.h"

using namespace std::placeholders;

void count(const std::vector<Polygon>& polygons, std::string name, std::ostream& out) {
    auto get_size = [](const Polygon& p) { return p.points.size(); };
    if (name == "EVEN") {
        out << std::count_if(polygons.begin(), polygons.end(),
            std::bind(std::equal_to<std::size_t>(), std::bind(std::modulus<std::size_t>(),
                std::bind(get_size, _1), 2), 0)) << "\n";
    }
    else if (name == "ODD") {
        out << std::count_if(polygons.begin(), polygons.end(),
            std::bind(std::not_equal_to<std::size_t>(), std::bind(std::modulus<std::size_t>(),
                std::bind(get_size, _1), 2), 0)) << "\n";
    }
    else {
        std::size_t pos = 0;
        int size = std::stoi(name, &pos);
        if (pos != name.length() || size < 3) {
            throw std::invalid_argument("");
        }
        out << std::count_if(polygons.begin(), polygons.end(),
            std::bind(std::equal_to<int>(),
                std::bind(get_size, _1), size)) << "\n";
    }
}

double getArea(const Polygon& polygon)
{
    const auto& pts = polygon.points;
    size_t n = pts.size();
    if (n < 3) return 0.0;
    double area = std::inner_product(
        pts.begin(), pts.end() - 1, pts.begin() + 1, 0.0, std::plus<double>(),
        [](const Point& a, const Point& b) {
            return static_cast<double>(a.x * b.y - a.y * b.x);
        }
    );
    area += static_cast<double>(pts.back().x * pts.front().y - pts.back().y * pts.front().x);
    return std::abs(area) / 2.0;
}

void area(const std::vector<Polygon>& polygon, std::string name, std::ostream& out) {
    iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1);
    if (name == "EVEN") {
        out << std::accumulate(polygon.begin(), polygon.end(), 0.0,
            [](double area, const Polygon& polygon) {
                if (polygon.points.size() % 2 == 0) {
                    return area + getArea(polygon);
                }
                return area;
            }) << "\n";
    }
    else if (name == "ODD") {
        out << std::accumulate(polygon.begin(), polygon.end(), 0.0,
            [](double area, const Polygon& polygon) {
                if (polygon.points.size() % 2 != 0) {
                    return area + getArea(polygon);
                }
                return area;
            }) << "\n";
    }
    else if (name == "MEAN") {
        if (polygon.empty()) {
            throw std::invalid_argument("");
        }
        out << std::accumulate(polygon.begin(), polygon.end(), 0.0,
            [](double area, const Polygon& polygon) {
                area += getArea(polygon);
                return area;
            }) / polygon.size() << "\n";
    }
    else {
        size_t target_size = std::stoul(name);
        if (target_size <= 2) throw std::invalid_argument("");
        out << std::accumulate(polygon.begin(), polygon.end(), 0.0,
            [target_size](double current_sum, const Polygon& p) {
                return (p.points.size() == target_size) ? current_sum + getArea(p) : current_sum;
            }) << "\n";
    }
}

void max(const std::vector<Polygon>& polygon, std::string name, std::ostream& out) {
    if (polygon.empty()) {
        throw std::invalid_argument("");
    }
    iofmtguard fmtguard(out);
    if (name == "AREA") {
        auto current = std::max_element(polygon.begin(), polygon.end(),
            std::bind(std::less<>{}, std::bind(getArea, _1), std::bind(getArea, _2)));
        out << std::setprecision(1) << std::fixed << getArea(*current) << "\n";
    }
    else if (name == "VERTEXES") {
        auto current = std::max_element(polygon.begin(), polygon.end(),
            std::bind(std::less<>{},
                std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, _1)),
                std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, _2))));
        out << current->points.size() << "\n";
    }
    else {
        throw std::invalid_argument("");
    }
}

void min(const std::vector<Polygon>& polygon, std::string name, std::ostream& out) {
    if (polygon.empty()) {
        throw std::invalid_argument("");
    }
    iofmtguard fmtguard(out);
    if (name == "AREA") {
        auto current = std::min_element(polygon.begin(), polygon.end(),
            std::bind(std::less<>{}, std::bind(getArea, _1), std::bind(getArea, _2)));
        out << std::setprecision(1) << std::fixed << getArea(*current) << "\n";
    }
    else if (name == "VERTEXES") {
        auto current = std::min_element(polygon.begin(), polygon.end(),
            std::bind(std::less<>{},
                std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, _1)),
                std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, _2))));
        out << current->points.size() << "\n";
    }
    else {
        throw std::invalid_argument("");
    }
}

void echo(std::vector<Polygon>& polygon, std::ostream& out) {
    Polygon temp;
    if (!(std::cin >> temp)) {
        throw std::invalid_argument("");
    }
    std::size_t count = std::count(polygon.begin(), polygon.end(), temp);
    std::vector<Polygon> result;
    result.reserve(polygon.size() + count);
    result = std::accumulate(polygon.begin(), polygon.end(), std::move(result),
        [&temp](std::vector<Polygon>& a, const Polygon& current) {
            a.push_back(current);
            if (current == temp) {
                a.push_back(current);
            }
            return a;
        });
    polygon = std::move(result);
    out << count << "\n";
}

bool isOrthogonal(const Point& a, const Point& b, const Point& c) {
    long long dx1 = a.x - b.x;
    long long dy1 = a.y - b.y;
    long long dx2 = c.x - b.x;
    long long dy2 = c.y - b.y;
    return (dx1 * dx2 + dy1 * dy2) == 0;
}

bool rightAngle(const Polygon& poly) {
    const auto& pts = poly.points;
    const size_t n = pts.size();
    if (n < 3) {
        throw std::invalid_argument("");
    }
    std::vector<size_t> idx(n);
    std::iota(idx.begin(), idx.end(), 0);
    return std::any_of(idx.begin(), idx.end(),
        [&](std::size_t i) {
            const Point& prev = pts[(i + n - 1) % n];
            const Point& curr = pts[i];
            const Point& next = pts[(i + 1) % n];
            return isOrthogonal(prev, curr, next);
        });
}

void rightshapes(const std::vector<Polygon>& polygon, std::ostream& out) {
    out << std::count_if(polygon.begin(), polygon.end(), rightAngle) << "\n";
}
