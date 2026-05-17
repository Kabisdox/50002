#include "PolygonCom.h"

using namespace std::placeholders;

int getShoelaceMultiplication(const Point& p1, const Point& p2)
{
    return (p1.x * p2.y - p1.y * p2.x);
}

double getArea(const Polygon& polygon)
{
    double area = 0.0;
    size_t size = polygon.points.size();
    for (size_t i = 0; i < size; i++) {
        area += getShoelaceMultiplication(polygon.points[i], polygon.points[(i + 1) % size]);
    }
    return std::abs(area) / 2.0;
}

void area(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
{
    iofmtguard guard(out);
    out << std::fixed << std::setprecision(1);

    if (mode == "EVEN") {
        out << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& polygon) {
                return sum + ((polygon.points.size() % 2 == 0) ? getArea(polygon) : 0.0);
            }) << "\n";
    }
    else if (mode == "ODD") {
        out << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& polygon) {
                return sum + ((polygon.points.size() % 2 != 0) ? getArea(polygon) : 0.0);
            }) << "\n";
    }
    else if (mode == "MEAN") {
        if (polygons.empty()) {
            throw std::invalid_argument("");
        }
        out << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& polygon) {
                return sum + getArea(polygon);
            }) / polygons.size() << "\n";
    }
    else {
        size_t size = std::stoul(mode);
        if (size <= 2) {
            throw std::invalid_argument("");
        }
        out << std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [size](double sum, const Polygon& polygon) {
                return sum + ((polygon.points.size() == size) ? getArea(polygon) : 0.0);
            }) << "\n";
    }
}

void max(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
{
    if (polygons.empty()) {
        throw std::invalid_argument("");
    }
    if (mode == "AREA") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            std::bind(std::less<double>{},
                std::bind(getArea, _1),
                std::bind(getArea, _2)));
        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    }
    else if (mode == "VERTEXES") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            std::bind(std::less<size_t>{},
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _1)),
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _2))));
        out << it->points.size() << "\n";
    }
    else {
        throw std::invalid_argument("");
    }
}

void min(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
{
    if (polygons.empty()) {
        throw std::invalid_argument("");
    }
    if (mode == "AREA") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            std::bind(std::less<double>{},
                std::bind(getArea, _1),
                std::bind(getArea, _2)));
        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    }
    else if (mode == "VERTEXES") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            std::bind(std::less<size_t>{},
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _1)),
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _2))));
        out << it->points.size() << "\n";
    }
    else {
        throw std::invalid_argument("");
    }
}

void count(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
{
    if (mode == "EVEN") {
        out << std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& polygon) {
                return polygon.points.size() % 2 == 0;
            }) << "\n";
    }
    else if (mode == "ODD") {
        out << std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& polygon) {
                return polygon.points.size() % 2 != 0;
            }) << "\n";
    }
    else {
        size_t size = std::stoul(mode);
        if (size <= 2) {
            throw std::invalid_argument("");
        }
        out << std::count_if(polygons.begin(), polygons.end(),
            [size](const Polygon& polygon) {
                return polygon.points.size() == size;
            }) << "\n";
    }
}

void lessarea(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon dest;
    in >> dest;
    if (in.fail() || polygons.empty()) {
        throw std::invalid_argument("");
    }
    out << std::count_if(polygons.begin(), polygons.end(),
        [&dest](const Polygon& polygon) {
            return getArea(polygon) < getArea(dest);
        }) << "\n";
}

void maxseq(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon dest;
    in >> dest;
    if (in.fail() || polygons.empty()) {
        throw std::invalid_argument("");
    }

    size_t maxCount = 0;
    auto it = polygons.begin();

    while (it != polygons.end()) {
        it = std::find(it, polygons.end(), dest);
        if (it == polygons.end()) break;

        auto endIt = std::find_if_not(it, polygons.end(),
            [&dest](const Polygon& p) { return p == dest; });

        size_t count = static_cast<size_t>(std::distance(it, endIt));
        maxCount = std::max(maxCount, count);
        it = endIt;
    }

    out << maxCount << "\n";
}