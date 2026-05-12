#include <iomanip>
#include <numeric>
#include <algorithm>
#include <functional>

#include "Polygon.h"
#include "Commands.h"
#include "Structs.h"

using namespace std::placeholders;

double getArea(const Polygon& polygon) {
    const auto& points = polygon.points;

    if (points.size() < 3) {
        return 0.0;
    }

    double area = std::inner_product(
        points.begin(),
        points.end() - 1,
        points.begin() + 1,
        0.0,
        std::plus<double>(),
        [](const Point& a, const Point& b) {
            return static_cast<double>(a.x * b.y - a.y * b.x);
        }
        );

    area += static_cast<double>(points.back().x * points.front().y - points.back().y * points.front().x);
    return std::abs(area) / 2.0;
}

// < AREA COMMAND >
void area(const std::vector<Polygon>& polygons, const std::string& name, std::ostream& os) {
    StreamGuard guard(os);
    os << std::fixed << std::setprecision(1);

    if (name == "EVEN") {
        os << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](const double area, const Polygon& p) {
                if (p.points.size() % 2 == 0) {
                    return area + getArea(p);
                }
                return area;
        }) << '\n';
    } else if (name == "ODD") {
        os << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](const double area, const Polygon& p) {
                if (p.points.size() % 2 != 0) {
                    return area + getArea(p);
                }
                return area;
        }) << '\n';
    } else if (name == "MEAN") {
        if (polygons.empty()) {
            throw std::invalid_argument("");
        }

        os << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](const double area, const Polygon& p) {
                return area + getArea(p);
            }) / polygons.size() << '\n';
    } else {
        std::size_t targetSize = std::stoul(name);
        if (targetSize < 3) {
            throw std::invalid_argument("");
        }

        os << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [&targetSize](const double area, const Polygon& p) {
                if (p.points.size() == targetSize) {
                    return area + getArea(p);
                }
                return area;
            }) << '\n';
    }
}

// < MIN COMMAND >
void min(const std::vector<Polygon>& polygons, const std::string& name, std::ostream& os) {
    if (polygons.empty()) {
        throw std::invalid_argument("");
    }

    StreamGuard guard(os);

    if (name == "AREA") {
        const auto currentElement = std::min_element(polygons.begin(), polygons.end(),
            std::bind(
                std::less<>{},
                std::bind(getArea, _1),
                std::bind(getArea, _2)
                ));
        os << std::fixed << std::setprecision(1) << getArea(*currentElement) << '\n';
    } else if (name == "VERTEXES") {
        const auto currentElement = std::min_element(polygons.begin(), polygons.end(),
            std::bind(std::less<>{},
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _1)),
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _2))
                ));
        os << currentElement->points.size() << '\n';
    } else {
        throw std::invalid_argument("");
    }
}

// < MAX COMMAND >
void max(const std::vector<Polygon>& polygons, const std::string& name, std::ostream& os) {
    if (polygons.empty()) {
        throw std::invalid_argument("");
    }

    StreamGuard guard(os);

    if (name == "AREA") {
        const auto currentElement = std::max_element(polygons.begin(), polygons.end(),
            std::bind(
                std::less<>{},
                std::bind(getArea, _1),
                std::bind(getArea, _2)
                ));
        os << std::fixed << std::setprecision(1) << getArea(*currentElement) << '\n';
    }
    else if (name == "VERTEXES") {
        const auto currentElement = std::max_element(polygons.begin(), polygons.end(),
            std::bind(std::less<>{},
                std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, _1)),
                std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, _2))
                ));
        os << currentElement->points.size() << '\n';
    }
    else {
        throw std::invalid_argument("");
    }
}

// < COUNT COMMAND >
void count(const std::vector<Polygon>& polygons, const std::string& name, std::ostream& os) {
    auto getPolygonSize = [](const Polygon& p) {
        return p.points.size();
    };

    if (name == "EVEN") {
        os << std::count_if(polygons.begin(), polygons.end(),
            std::bind(
                std::equal_to<std::size_t>(),
                std::bind(std::modulus<std::size_t>(),
                std::bind(getPolygonSize, _1), 2), 0)
                ) << '\n';
    }
    else if (name == "ODD") {
        os << std::count_if(polygons.begin(), polygons.end(),
            std::bind(
                std::not_equal_to<std::size_t>(),
                std::bind(std::modulus<std::size_t>(),
                std::bind(getPolygonSize, _1), 2), 0)
                ) << '\n';
    }
    else {
        std::size_t position = 0;
        int size = std::stoi(name, &position);

        if (position != name.length() || size < 3) {
            throw std::invalid_argument("");
        }

        os << std::count_if(polygons.begin(), polygons.end(),
            std::bind(
                std::equal_to<int>(),
                std::bind(getPolygonSize, _1),
                size)
                ) << '\n';
    }
}

// < RMECHO COMMAND >
void rmecho(std::vector<Polygon>& polygons, const Polygon& target, std::ostream& os) {
    const auto oldSize = polygons.size();

    const auto newEnd = std::unique(
        polygons.begin(),
        polygons.end(),
        [&target](const Polygon& lhs, const Polygon& rhs) {
            return lhs == target && rhs == target;
        }
    );

    polygons.erase(newEnd, polygons.end());
    os << oldSize - polygons.size() << '\n';
}

// < INTERSECTIONS COMMAND >
void intersections(const std::vector<Polygon>& polygons, const Polygon& target, std::ostream& os) {
    os << std::count_if(
        polygons.begin(),
        polygons.end(),
        [&target](const Polygon& polygon) {
            return polygonsIntersect(polygon, target);
        }) << '\n';
}
